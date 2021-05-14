//
//  thread_monitor.cpp
//  wmp_toolkit
//
//  Created by mjzheng on 2021/4/20.
//  Copyright © 2021年 mjzheng. All rights reserved.
//

#include "monitor_interface.h"
#include <float.h>

#include <string>
#include <unordered_map>
#include <map>
#include <sstream>
#include <memory>

#include "thread_cpu_info.h"
#include "config_center.h"
#include "common_def.h"

namespace IDbg {

constexpr static int kRangeGroup = 51;

struct CpuRangeInfo {
  int32_t count = 0;
  // float total = 0 ;
};

static const char* kRangeNameList[kRangeGroup] = {
  "range_0_2",
  "range_2_4",
  "range_4_6",
  "range_6_8",
  "range_8_10",
  "range_10_12",
  "range_12_14",
  "range_14_16",
  "range_16_18",
  "range_18_20",
  "range_20_22",
  "range_22_24",
  "range_24_26",
  "range_26_28",
  "range_28_30",
  "range_30_32",
  "range_32_34",
  "range_34_36",
  "range_36_38",
  "range_38_40",
  "range_40_42",
  "range_42_44",
  "range_44_46",
  "range_46_48",
  "range_48_50",
  "range_50_52",
  "range_52_54",
  "range_54_56",
  "range_56_58",
  "range_58_60",
  "range_60_62",
  "range_62_64",
  "range_64_66",
  "range_66_68",
  "range_68_70",
  "range_70_72",
  "range_72_74",
  "range_74_76",
  "range_76_78",
  "range_78_80",
  "range_80_82",
  "range_82_84",
  "range_84_86",
  "range_86_88",
  "range_88_90",
  "range_90_92",
  "range_92_94",
  "range_94_96",
  "range_96_98",
  "range_98_100",
  "range_0"  // special
};

struct ThreadCpuInfo {
  std::string name;
  std::string module;
  uint64_t th = 0;
  float total = 0;
  float cpu_max = 0;
  float cpu_min = 101;
  int32_t count = 0;
  int32_t valid_count = 0;
  CpuRangeInfo range[kRangeGroup] = {};
};

typedef std::unordered_map<uint64_t, ThreadCpuInfo> ThreadCpuInfoArray;

static void ReportThreadInfo(const ThreadCpuInfo& thread) {
  if (thread.valid_count == 0) {
    //LOG("filter report thread info ");
    return;
  }

  auto cpu_avg = thread.count > 0 ? thread.total / thread.count : 0.0;
  auto cpu_valid_avg = thread.valid_count > 0 ? thread.total / thread.valid_count : 0.0;
  std::map<std::string, std::string> event_properties;

  // basic
  event_properties["th_module"] = thread.module;
  event_properties["th_name"] = thread.name;
  event_properties["th_cpu_min"] = std::to_string(thread.cpu_min);
  event_properties["th_cpu_max"] = std::to_string(thread.cpu_max);
  event_properties["th_cpu_avg"] = std::to_string(cpu_avg);
  event_properties["th_cpu_valid_vag"] = std::to_string(cpu_valid_avg);
  event_properties["th_count"] = std::to_string(thread.count);
  event_properties["th_valid_count"] = std::to_string(thread.valid_count);
  event_properties["th_total"] = std::to_string(thread.total);
  std::stringstream ss;
  for (int i=0; i < kRangeGroup; ++i) {
    auto& r = thread.range[i];
    if (r.count > 0) {
      const char* buf = kRangeNameList[i];
      event_properties[buf] = std::to_string(r.count);
    }
    ss << r.count << " ";
  }
  std::string range_str = ss.str();
  
  LOG("mj thread info min[%2f] count[%d] valid[%d] avg[%.2f] valid avg[%.2f] max[%.2f] module[%s] name[%s] rang[%s]", thread.cpu_min,
      thread.count, thread.valid_count, cpu_avg, cpu_valid_avg, thread.cpu_max,
      thread.module.c_str(), thread.name.c_str(), ss.str().c_str());
}

static void ReportThreadRangeInfo(const ThreadCpuInfo& thread) {
  if (thread.valid_count == 0 || thread.name == "") {
    return;
  }
  for (int i=0; i < kRangeGroup; ++i) {
    auto& r = thread.range[i];
    if (r.count == 0) continue;
    std::map<std::string, std::string> event_properties;
    event_properties["th_name"] = thread.name;
    const char* buf = kRangeNameList[i];
    event_properties["range_name"] = buf;
    event_properties["range_count"] = std::to_string(r.count);
  }
}

class ThreadMonitorImpl : public MonitorInterface {
 public:
  ThreadMonitorImpl();

  ~ThreadMonitorImpl();

 public:
  int Start() override;

  void Stop() override;
  
  void OnTimer() override;

 private:
  ThreadCpuInfoArray threads_cpu_info_;
};

std::unique_ptr<MonitorInterface> CreateThreadCpuMonitor() {
  std::make_unique<ThreadMonitorImpl>();
}

static const char* business_module[] = {"xnn", "wemeet_base", "wemeet_sdk_internal", "caulk", "WeMeetApp", "ImSDK", "xcast"};

ThreadMonitorImpl::ThreadMonitorImpl()
    : config_(std::make_unique<ThreadCpuConfig>()) {
}

ThreadMonitorImpl::~ThreadMonitorImpl() {
  Stop();
}

int ThreadMonitorImpl::Start() {
  if (!config_->is_monitor) {
    return 1;
  }
  threads_cpu_info_.clear();
  return 0;
}

void ThreadMonitorImpl::Stop() {
  for (auto& item : threads_cpu_info_) {
    ReportThreadInfo(item.second);
    if (config_->is_report_range) {
      ReportThreadRangeInfo(item.second);
    }
  }
  threads_cpu_info_.clear();
}

static void UpdateThreadCpuInfo(const IDbg::ThreadStack& sample, ThreadCpuInfo* info) {
  ++info->count;
  auto& cpu = sample.cpu;
  //LOG("update thread cpu info");
  if (cpu > FLT_EPSILON) {
    info->total += cpu;
    ++info->valid_count;
    if (info->cpu_min > cpu) {
      info->cpu_min = cpu;
    }
    if (info->cpu_max < cpu) {
      info->cpu_max = cpu;
    }
    auto index = static_cast<int>(floor(cpu));
    if (index >= 0 && index <= 99) {
      index /= 2;
      ++info->range[index].count;
    }
  } else {
    ++info->range[kRangeGroup - 1].count;
  }
}

void ThreadMonitorImpl::OnTimer() {
  ThreadStackArray cpu_info;
  int ret = GetAllThreadInfo(ThreadOptions::kBasic, &cpu_info);
  if (ret != 0) {
    return;
  }
  for (auto& thread : cpu_info) {
    uint64_t th = static_cast<uint64_t>(thread.th);
    auto iter = threads_cpu_info_.find(th);
    if (iter != threads_cpu_info_.end()) {
      auto& thread_cpu_info = iter->second;
      UpdateThreadCpuInfo(thread, &thread_cpu_info);
    } else {
      ThreadCpuInfo thread_cpu_info;
      thread_cpu_info.name = thread.name;
      thread_cpu_info.th = static_cast<uint64_t>(thread.th);
//      GetThreadModuleName(thread, business_module);
//      thread_cpu_info.module = thread.module;
      UpdateThreadCpuInfo(thread, &thread_cpu_info);
      threads_cpu_info_.insert(std::make_pair(th, thread_cpu_info));
    }
  }
}

}  // namespace IDbg
