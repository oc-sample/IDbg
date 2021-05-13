//
//  cpu_usage_monitor_config.h
//
//  Created by mjzheng on 2021/3/18.
//

#ifndef IDBG_CONFIG_CENTER_H_
#define IDBG_CONFIG_CENTER_H_

#include <map>
#include <string>

namespace IDbg {

struct CpuConditionConfig {
  int64_t duration;
  float threshold;
  float max_threshold;
  std::string cpu_type;
};

struct HighCpuConfig {
  bool enable = true;
  int64_t modulus_base = 1;
  
  CpuConditionConfig app_condition = {10000, 45, 90, "app"};
  CpuConditionConfig sys_condition = {10000, 85, 99, "sys"};
  float thread_cpu_threshold = 1;
  
  int64_t dump_count = 3;
  int64_t dump_interval_ms = 500;
  int64_t dump_thread_number = 3;
  
  int64_t max_sample_count = 1;
  int64_t sample_interval_ms = 60000;
  int64_t detect_interval_ms = 2000;
  
  std::map<std::string, int> block_thread_list;
};

struct ThreadMonitorConfig {
  bool is_monitor = true;
  bool is_report_range = false;
  uint64_t interval = 2000;
};


}  // namespace IDBG_CONFIG_CENTER_H_

#endif  // IDBG_CONFIG_CENTER_H_
