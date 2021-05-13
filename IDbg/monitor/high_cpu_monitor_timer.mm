//
//  TaskTimer.m
//  iPhone_1
//
//  Created by mjzheng on 2020/10/12.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#import "high_cpu_monitor_timer.h"
#include "high_cpu_monitor.h"
#include <memory>

@interface HighCpuMonitorTimer() {
  std::unique_ptr<IDbg::CpuUsageMonitor> monitor_;
}
@property(nonatomic, strong)NSTimer* timer;
@end

@implementation HighCpuMonitorTimer

- (instancetype)init {
  self = [super init];
  return self;
}

-(void)start {
  if (nullptr == monitor_) {
    //monitor_ = IDbg::CreateThreadMonitor();
  }
  if (nullptr != monitor_) {
    if (monitor_->Start() != 0) {
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
  
  if (nullptr != monitor_) {
    monitor_->Stop();
  }
}

-(void)onTimer{
  NSLog(@"on thread monitor timer");
  if (nullptr != monitor_) {
    monitor_->OnTimer();
  }
}

@end
