//
//  TaskTimer.m
//  iPhone_1
//
//  Created by mjzheng on 2020/10/12.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#import "task_timer.h"
#include "thread_cpu_monitor.h"
#include <memory>

@interface TaskTimer() {
  std::unique_ptr<IDbg::ThreadMonitor> thread_monitor_;
}
@end

@implementation TaskTimer

- (instancetype)init {
  self = [super init];
  if (self != nil) {
    thread_monitor_ = IDbg::CreateThreadMonitor();
  }
  return self;
}

-(void)startTimer {
    self.timer = [NSTimer timerWithTimeInterval:1 target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
    //[[NSRunLoop currentRunLoop] addTimer:self.timer forMode:NSRunLoopCommonModes]; // 一直有效
    [[NSRunLoop currentRunLoop] addTimer:self.timer forMode:NSDefaultRunLoopMode]; // 默认mode
  
  if (nullptr != thread_monitor_) {
    thread_monitor_->Start();
  }
}

-(void)stopTimer {
    [self.timer invalidate];
    _timer = nil;
  
  if (nullptr != thread_monitor_) {
    thread_monitor_->Stop();
  }
}

-(void)onTimer{
  if (nullptr != thread_monitor_) {
    thread_monitor_->OnTimer();
  }
}

@end
