//
// Created by mjzheng on 2021/3/18.
//  Copyright © 2021年 mjzheng. All rights reserved.
//

#include "high_cpu_monitor.h"
#include "thread_cpu_info.h"
#include "config_center.h"

namespace IDbg {

#if defined(__LP64__)
#define TRACE_FMT         "%d %s 0x%016lx 0x%016lx"
#else
#define TRACE_FMT         "%d %s 0x%08lx 0x%08lx"
#endif

#define THREAD_FRAME_BUF_SIZE 2048

using UploadStackDelegate = std::function<void(const std::string& json_data)>;

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
  
private:
  uint64_t self_tiny_id_ = 0;
  uint64_t meeting_id_ = 0;
  
private:
  int cur_sample_count_ = 0;
  uint64_t last_sample_time_ = 0;
  
private:
  uint64_t app_cpu_begin_time_ = 0;
  uint64_t sys_cpu_begin_time_ = 0;
  
private:
  std::unique_ptr<HighCpuConfig> config_;
  UploadStackDelegate upload_;
  
private:
  float app_cpu_;
  float sys_cpu_;
  int cpu_core_;
};


CpuUsageMonitorApple::CpuUsageMonitorApple(UploadStackDelegate delegate) {
  config_ = std::make_unique<HighCpuConfig>();
  upload_ = delegate;
  cpu_core_ = IDbg::GetCpuCore();
}

CpuUsageMonitorApple::~CpuUsageMonitorApple() {
  Stop();
}

void CpuUsageMonitorApple::Run(uint64_t self_tiny_id, uint64_t meeting_id) {
//  if (!config_->IsEnable(self_tiny_id)) {
//    return;
//  }
  // call when join room
  self_tiny_id_ = self_tiny_id;
  meeting_id_ = meeting_id;
  cur_sample_count_ = 0;
}

void CpuUsageMonitorApple::Stop() {
}

int CpuUsageMonitorApple::SampleFrequencyControl(uint64_t current_time) {
  if (cur_sample_count_ >= config_->max_sample_count) {
    return 1;
  }
  
  if (current_time - last_sample_time_ < config_->sample_interval_ms) {
    return 2;
  }
  return 0;
}

int CpuUsageMonitorApple::SampleConditionControl(uint64_t current_time) {
  auto condition_control = [](CpuConditionConfig& condition, float cpu, uint64_t current_time, uint64_t& begin_time) {
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
  if (condition_control(config_->app_condition, app_cpu_, current_time, app_cpu_begin_time_) != 0 &&
      condition_control(config_->sys_condition, sys_cpu_, current_time, sys_cpu_begin_time_) != 0) {
    return 1;
  }
  return 0;
}

void CpuUsageMonitorApple::OnTimer() {
  uint64_t current_time = 0;
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
  ThreadStackArray cpu_info;
  int ret = GetAllThreadInfo(IDbg::ThreadOptions::kBasic, &cpu_info);
  if (ret != 0) {
    return;
  }
  
  std::sort(cpu_info.begin(), cpu_info.end(), [](const IDbg::ThreadStack& v1, const IDbg::ThreadStack& v2) {
    return v1.cpu > v2.cpu;
  });
  
  ThreadIdArray thread_list;
  for (size_t i = 0; i < config_->dump_thread_number && i < cpu_info.size(); ++i) {
    auto& info = cpu_info[i];
    if (info.cpu < config_->thread_cpu_threshold) {
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
  std::vector<ThreadStackArray> time_slices;
  for (int i=0; i < config_->dump_count; ++i) {
    ThreadStackArray ls;
    GetThreadInfoById(thread_list, ThreadOptions::kFrames, &ls);
    std::sort(ls.begin(), ls.end(), [](const IDbg::ThreadStack& v1, const IDbg::ThreadStack& v2) {
      return v1.cpu > v2.cpu;
    });
    time_slices.push_back(ls);
  }
  
//  std::string json_data;
//  GetUploadParams(time_slices, json_data);
//  upload_(json_data);
}

}  // namespace IDbg
