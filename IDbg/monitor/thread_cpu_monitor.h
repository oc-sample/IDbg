//
//  thread_monitor.h
//  AppCommon
//
//  Created by mjzheng on 2021/4/20.
//  Copyright © 2021年 mjzheng. All rights reserved.
//

#ifndef IDBG_THREAD_MONITOR_H_
#define IDBG_THREAD_MONITOR_H_

#include <memory>

namespace IDbg {
class ThreadMonitor {
 public:
  virtual ~ThreadMonitor() = default;
 public:
  virtual int Start() = 0;
  virtual void Stop() = 0;
  virtual void OnTimer() = 0;
};

std::unique_ptr<ThreadMonitor> CreateThreadMonitor();

}  // namespace IDbg

#endif  // IDBG_THREAD_MONITOR_H_
