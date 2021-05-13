//
//  TaskTimer.m
//  iPhone_1
//
//  Created by mjzheng on 2020/10/12.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#import "thread_cpu_monitor_timer.h"
#include "thread_cpu_monitor.h"
#include <memory>

@interface ThreadCpuMonitorTimer() {
  std::unique_ptr<IDbg::ThreadMonitor> thread_monitor_;
}
@property(nonatomic, strong)NSTimer* timer;
@end

@implementation ThreadCpuMonitorTimer

- (instancetype)init {
  self = [super init];
  return self;
}

-(void)start {
  if (nullptr == thread_monitor_) {
    thread_monitor_ = IDbg::CreateThreadMonitor();
  }
  if (nullptr != thread_monitor_) {
    if (thread_monitor_->Start() != 0) {
      NSLog(@"thread monitor is disable");
      return;
    }
  }
  self.timer = [NSTimer timerWithTimeInterval:1 target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
  [[NSRunLoop currentRunLoop] addTimer:self.timer forMode:NSRunLoopCommonModes]; // 一直有效
  //[[NSRunLoop currentRunLoop] addTimer:self.timer forMode:NSDefaultRunLoopMode]; // 默认mode
}

-(void)stop {
  [self.timer invalidate];
  _timer = nil;
  
  if (nullptr != thread_monitor_) {
    thread_monitor_->Stop();
  }
}

-(void)onTimer{
  NSLog(@"on thread monitor timer");
  if (nullptr != thread_monitor_) {
    thread_monitor_->OnTimer();
  }
}

@end
