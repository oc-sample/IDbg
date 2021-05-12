//
// Created by mjzheng on 2021/3/18.
//  Copyright © 2021年 mjzheng. All rights reserved.
//

#ifndef IDBG_HIGH_CPU_MONITOR_H_
#define IDBG_HIGH_CPU_MONITOR_H_

#include <stdint.h>

namespace IDbg {
class CpuUsageMonitor {
public:
  virtual ~CpuUsageMonitor() = default;

  virtual void Run(uint64_t self_tiny_id, uint64_t meeting_id) = 0;

  virtual void Stop() = 0;
};

}  // namespace IDbg

#endif  // IDBG_HIGH_CPU_MONITOR_H_
