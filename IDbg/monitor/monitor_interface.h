//
// Created by mjzheng on 2021/3/18.
//  Copyright © 2021年 mjzheng. All rights reserved.
//

#ifndef IDBG_HIGH_CPU_MONITOR_H_
#define IDBG_HIGH_CPU_MONITOR_H_

#include <stdint.h>
#include <memory>

namespace IDbg {
class MonitorInterface {
public:
  virtual ~MonitorInterface() = default;

    virtual int Start() = 0;
    virtual void Stop() = 0;
    virtual void OnTimer() = 0;
};

std::unique_ptr<MonitorInterface> CreateMonitor();

}  // namespace IDbg

#endif  // IDBG_HIGH_CPU_MONITOR_H_
