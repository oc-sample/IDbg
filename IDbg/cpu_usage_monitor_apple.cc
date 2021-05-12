//
//  cpu_usage_monitor_apple.cpp
//  wmp_toolkit
//
//  Created by mjzheng on 2021/3/10.
//

#include "cpu_usage_monitor.h"
#include <base/core/include/analytics.h>
#include <base/common/include/convert_util.h>
#include <base/common/include/string_util.h>
#include <base/common/include/log_writer.h>
#include <base/common/include/time_util.h>
#include <wmp/runtime/utils.h>
#include <wmp/runtime/runtime.h>
#include <iomanip>
#include "thread_cpu_info.h"
#include "cpu_usage_monitor_config.h"
#include "thread_model.h"
#include "mini_dump_file.h"


BEGIN_NAMESPACE_WMP_TOOLKIT


#if defined(__LP64__)
#define TRACE_FMT         "%d %s 0x%016lx 0x%016lx"
#else
#define TRACE_FMT         "%d %s 0x%08lx 0x%08lx"
#endif

#define THREAD_FRAME_BUF_SIZE 2048

class CpuUsageMonitorApple : public CpuUsageMonitor {
public:
  explicit CpuUsageMonitorApple(UploadStackDelegate delegate);
  
  virtual ~CpuUsageMonitorApple();
  
public: // in timer thread
  void Run(uint64_t self_tiny_id, uint64_t meeting_id) override;
  
  void Stop() override;
  
private: // in work thread
  void OnTimer();
  
  int SampleFrequencyControl(uint64_t current_time);
  
  int SampleConditionControl(uint64_t current_time);
  
  void SampleStack(uint64_t current_time);
  
  void DumpStackMulti(const IDbg::ThreadIdArray& thread_list);
  
  float GetAppCpu();
  
  float GetSysCpu();
  void GetUploadParams(const std::vector<IDbg::ThreadStackArray>& time_slices, std::string& result);
  
private:
  uint64_t self_tiny_id_ = 0;
  uint64_t meeting_id_ = 0;
  base::unique_ptr<BASE_THREADING::Timer> timer_;
  
private:
  int cur_sample_count_ = 0;
  uint64_t last_sample_time_ = 0;
  
private:
  uint64_t app_cpu_begin_time_ = 0;
  uint64_t sys_cpu_begin_time_ = 0;
  
private:
  std::unique_ptr<CpuMonitorConfig> config_;
  std::unique_ptr<IDbg::ThreadModel> work_thread_;
  
  UploadStackDelegate upload_;
  
private:
  float app_cpu_;
  float sys_cpu_;
  int cpu_core_;
};

base::unique_ptr<CpuUsageMonitor> CreateCpuUsageMonitor(UploadStackDelegate delegate) {
  return base::make_unique<CpuUsageMonitorApple>(delegate);
}

CpuUsageMonitorApple::CpuUsageMonitorApple(UploadStackDelegate delegate) {
  config_ = std::make_unique<CpuMonitorConfig>();
  work_thread_ = std::make_unique<IDbg::ThreadModel>("cpu_monitor");
  upload_ = delegate;
  cpu_core_ = IDbg::GetCpuCore();
  log_info << "cpu core " << cpu_core_;
}

CpuUsageMonitorApple::~CpuUsageMonitorApple() {
  Stop();
}

void CpuUsageMonitorApple::Run(uint64_t self_tiny_id, uint64_t meeting_id) {
  if (!config_->IsEnable(self_tiny_id)) {
    return;
  }
  // call when join room
  self_tiny_id_ = self_tiny_id;
  meeting_id_ = meeting_id;
  cur_sample_count_ = 0;
  
  if (!timer_) {
    timer_ = BASE_THREADING::CreateTimer(BASE_THREADING::LooperThread::WorkerLooper(), [this] {
      work_thread_->PushTask([this] {
        OnTimer();
      });
    });
    timer_->Start(config_->detect_interval_ms_, true);
    log_info << "monitor run.";
  }
}

void CpuUsageMonitorApple::Stop() {
  // call when leave room
  if (timer_) {
    timer_->Stop();
    timer_.reset();
    log_info << "monitor stop";
  }
}

int CpuUsageMonitorApple::SampleFrequencyControl(uint64_t current_time) {
  log_debug << "sample count " << cur_sample_count_ << " max " << config_->max_sample_count_
            << " current time " << current_time << " last time " << last_sample_time_;
  if (cur_sample_count_ >= config_->max_sample_count_) {
    return 1;
  }
  
  if (current_time - last_sample_time_ < config_->sample_interval_ms_) {
    return 2;
  }
  return 0;
}

int CpuUsageMonitorApple::SampleConditionControl(uint64_t current_time) {
  auto condition_control = [](CpuConditionConfig& condition, float cpu, uint64_t current_time, uint64_t& begin_time) {
    log_debug << "cpu type " << condition.cpu_type << " " << cpu << " threshold "<< condition.threshold;
    if (cpu < condition.threshold) {
      begin_time = 0;
      return 1;
    }
    
    if (begin_time == 0) {
      begin_time = current_time;
    }
    
    if (current_time - begin_time < condition.duration) {
      return 2;
    }
    if (cpu > condition.max_threshold) {
      return 3;
    }
    return 0;
  };
  if (condition_control(config_->app_condition_, app_cpu_, current_time, app_cpu_begin_time_) != 0 &&
      condition_control(config_->sys_condition_, sys_cpu_, current_time, sys_cpu_begin_time_) != 0) {
    return 1;
  }
  return 0;
}

inline float CpuUsageMonitorApple::GetAppCpu() {
  float app_cpu = IDbg::GetAppCpu() / cpu_core_;
  return app_cpu;
}

void CpuUsageMonitorApple::OnTimer() {
  uint64_t current_time = UTIL_TIME::GetTimestampMsec();
  if (0 != SampleFrequencyControl(current_time)) {
    return;
  }
  app_cpu_ = GetAppCpu();
  sys_cpu_ = GetSysCpu();
  if (0 != SampleConditionControl(current_time)) {
    return;
  }
  
  ++cur_sample_count_;
  SampleStack(current_time);
}

void CpuUsageMonitorApple::SampleStack(uint64_t current_time) {
  IDbg::ThreadStackArray cpu_info;
  int ret = IDbg::GetAllThreadInfo(cpu_info, IDbg::ThreadOptions::kBasic);
  if (ret != 0) {
    return;
  }
  
  std::sort(cpu_info.begin(), cpu_info.end(), [](const IDbg::ThreadStack& v1, const IDbg::ThreadStack& v2) {
    return v1.cpu > v2.cpu;
  });
  
  IDbg::ThreadIdArray thread_list;
  for (size_t i = 0; i < config_->dump_thread_number_ && i < cpu_info.size(); ++i) {
    auto& info = cpu_info[i];
    if (info.cpu < config_->thread_cpu_threshold_) {
      break;
    }
      thread_list.push_back(info.th);
  }
  if (thread_list.empty()) {
    return;
  }
  last_sample_time_ = current_time;
  DumpStackMulti(thread_list);
}

void CpuUsageMonitorApple::DumpStackMulti(const IDbg::ThreadIdArray& thread_list) {
  std::vector<IDbg::ThreadStackArray> time_slices;

  for (int i=0; i < config_->dump_count_; ++i) {
    IDbg::ThreadStackArray ls;
    GetThreadInfoById(ls, thread_list, IDbg::ThreadOptions::kFrames);
    std::sort(ls.begin(), ls.end(), [](const IDbg::ThreadStack& v1, const IDbg::ThreadStack& v2) {
      return v1.cpu > v2.cpu;
    });
    log_info << "samle thread size " << ls.size();
    time_slices.push_back(ls);
    int ret = work_thread_->Sleep(config_->dump_interval_ms_);
    if (ret != 0) { // exit
      return;
    }
  }
  
  std::string json_data;
  GetUploadParams(time_slices, json_data);
  //log_info << "json data " << json_data.c_str();
  upload_(json_data);
}

inline float CpuUsageMonitorApple::GetSysCpu() {
  return IDbg::GetSysCpu();
}

void CpuUsageMonitorApple::GetUploadParams(const std::vector<IDbg::ThreadStackArray>& time_slices, std::string& result) {
  char buf[THREAD_FRAME_BUF_SIZE];
  BASE_JSON::JsonValue slices; // array
  for (auto& time_slice : time_slices) {
    BASE_JSON::JsonValue threads; // arrary
    for (auto& item : time_slice) {
      BASE_JSON::JsonValue thread; // map
      thread.SetJsonObject("id", BASE_JSON::JsonValue(std::to_string(item.th)));
      log_info << "thread id " << item.th;
      thread.SetJsonObject("name", BASE_JSON::JsonValue(item.name));
      thread.SetJsonObject("cpu_usage", BASE_JSON::JsonValue(item.cpu));
      BASE_JSON::JsonValue frames; // array
      for (auto& frame : item.frames) {
        snprintf(buf, THREAD_FRAME_BUF_SIZE, TRACE_FMT, frame.index, frame.module_name.c_str(), frame.module_base,
                 (uintptr_t)frame.address);
        frames.AddaJsonObject(BASE_JSON::JsonValue(buf));
      }
      BASE_JSON::JsonValue calls;
      calls.SetJsonObject("calls", frames);
      thread.SetJsonObject("frame", calls);
      threads.AddaJsonObject(thread);
    }
    BASE_JSON::JsonValue slice; // map
    slice.SetJsonObject("threads", threads);
    slice.SetJsonObject("sys_cpu", BASE_JSON::JsonValue(sys_cpu_));
    slice.SetJsonObject("app_cpu", BASE_JSON::JsonValue(app_cpu_));
    slice.SetJsonObject("meeting_id", BASE_JSON::JsonValue(int64_t(meeting_id_)));
    slice.SetJsonObject("self_tiny_id", BASE_JSON::JsonValue(int64_t(self_tiny_id_)));
    slice.SetJsonObject("duration", BASE_JSON::JsonValue(0));
    slices.AddaJsonObject(slice);
  }
  result = slices.ToStyledString();
}

END_NAMESPACE_WMP_TOOLKIT
