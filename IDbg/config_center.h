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

struct CpuMonitorConfig {
  bool enable_ = true;
  int64_t modulus_base_ = 1;
  
  CpuConditionConfig app_condition_ = {10000, 45, 90, "app"};
  CpuConditionConfig sys_condition_ = {10000, 85, 99, "sys"};
  float thread_cpu_threshold_ = 1;
  
  int64_t dump_count_ = 3;
  int64_t dump_interval_ms_ = 500;
  int64_t dump_thread_number_ = 3;
  
  int64_t max_sample_count_ = 1;
  int64_t sample_interval_ms_ = 60000;
  int64_t detect_interval_ms_ = 2000;
  
  std::map<std::string, int> block_thread_list_;
};

}  // namespace IDBG_CONFIG_CENTER_H_

#endif  // IDBG_CONFIG_CENTER_H_
