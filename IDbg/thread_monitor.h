//
//  thread_monitor.h
//  AppCommon
//
//  Created by mjzheng on 2021/4/20.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#ifndef IDBG_THREAD_MONITOR_H_
#define IDBG_THREAD_MONITOR_H_

namespace IDbg {
class ThreadMonitor {
 public:
  virtual ~ThreadMonitor() = default;
 public:
  virtual void Start() = 0;
  virtual void Stop() = 0;
};
}  // namespace IDbg

#endif  // IDBG_THREAD_MONITOR_H_
