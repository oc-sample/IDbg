//
//  cpu_usage_monitor_config.h
//  AppCommon
//
//  Created by mjzheng on 2021/3/18.
//

#ifndef cpu_usage_monitor_config_h
#define cpu_usage_monitor_config_h

#include <base/common/include/json_value.h>
#include <wmp/runtime/runtime.h>
#include <map>

BEGIN_NAMESPACE_WMP_TOOLKIT

struct CpuConditionConfig {
  int64_t duration;
  float threshold;
  float max_threshold;
  std::string cpu_type;
};

class CpuMonitorConfig : public WEMEET_SDK::IConfigureValueDelegate {
public:
  CpuMonitorConfig();
  
  virtual ~CpuMonitorConfig();
  
public:
  bool IsEnable(uint64_t self_tiny_id);
  
public: // WEMEET_SDK::IConfigureValueDelegate
  void OnConfigureChange(const char* id, const char* configure, std::size_t len, const WEMEET_SDK::ConfigureState& state) override;
  
private:
  bool IsEnableMonitor(const BASE_JSON::JsonValue& config);
  
  bool IsHitDeviceList(const BASE_JSON::JsonValue& device_list, BASE_JSON::JsonValue& hit_device);
  
  bool IsHitVersionList(const BASE_JSON::JsonValue& version_list, bool bDefault);
  
  void GetBlockThreadList(const BASE_JSON::JsonValue& ls);
  
  void GetFloatConfig(const BASE_JSON::JsonValue& config, float& value);
  
  void ParseConfig(const char* configure, std::size_t len);
  
public:
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

END_NAMESPACE_WMP_TOOLKIT

#endif /* cpu_usage_monitor_config_h */
