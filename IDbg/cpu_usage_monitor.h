#ifndef WMP_TOOLKIT_CPU_USAGE_MONITOR_H
#define WMP_TOOLKIT_CPU_USAGE_MONITOR_H

#include <wmp/include/wmp_namespace.h>
#include <base/common/include/base_header.h>
#include "cpu_usage_ipc_bridge.h"
#include <include/public/wemeet_sdk_lib.h>
#include <include/internal/extensions/tool_service_interface.h>

#define DEFAULT_TOP_PROCS 5
#define DEFUALT_INTERVAL_SECS 60
#define DEFUALT_CALC_SECS 5

BEGIN_NAMESPACE_WMP_TOOLKIT

class CpuUsageMonitor {
public:
  struct Request {
    explicit Request(int top_procs = DEFAULT_TOP_PROCS, int intervals_secs = DEFUALT_INTERVAL_SECS, int calc_secs = DEFUALT_CALC_SECS):
      top_procs(top_procs),
      intervals_secs(intervals_secs),
      calc_secs(calc_secs) {}

    int top_procs;
    int intervals_secs;
    int calc_secs;
  };

public:
  virtual ~CpuUsageMonitor() = default;

  virtual void Run(uint64_t self_tiny_id, uint64_t meeting_id) = 0;

  virtual void Stop() = 0;
};

using CpuUsageMonitorDelegate = std::function<
  void(CpuUsageMonitor*, CpuUsage::Response*)>;

base::unique_ptr<CpuUsageMonitor> CreateCpuUsageMonitor(
  base::unique_ptr<CpuUsageMonitor::Request> request,
  CpuUsageMonitorDelegate delegate);

base::unique_ptr<CpuUsageMonitor> CreateCpuUsageMonitor();

#ifdef OS_IOS
using UploadStackDelegate = std::function<void(const std::string& json_data)>;
base::unique_ptr<CpuUsageMonitor> CreateCpuUsageMonitor(UploadStackDelegate delegate); // ios
#endif
END_NAMESPACE_WMP_TOOLKIT

#endif
