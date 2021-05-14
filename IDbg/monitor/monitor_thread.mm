//
//  Heartbeat.m
//  iPhone_1
//
//  Created by mjzheng on 2020/10/12.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#import "monitor_thread.h"
#import "monitor_timer.h"
#import "monitor_interface.h"
#import "config_center.h"

@interface MonitorThread()<NSPortDelegate>
@property(nonatomic, strong)NSThread* thread;
@property(nonatomic, strong)NSPort* port;
@property(nonatomic, strong)MonitorTimer* highCpuMonitorTimer;
@property(nonatomic, strong)MonitorTimer* threadCpuMonitorTimer;

@end

@implementation MonitorThread

- (instancetype)init {
  self = [super init];
  return self;
}

- (void)start{
  NSLog(@"start thread");
  if (self.thread == nil) {
    self.thread = [[NSThread alloc] initWithTarget:self selector:@selector(run) object:nil];
    self.thread.name = @"data_report";
    [self.thread start];
    NSLog(@"start new thread ");
  }
}

- (void)stop{
  NSLog(@"stop thread");
  if (self.thread != nil) {
    [self performSelector:@selector(onStop) onThread:self.thread withObject:nil waitUntilDone:YES];
    _thread = nil;
  }
}

- (void)run {
  NSLog(@"before run");
  
  self.highCpuMonitorTimer = [[MonitorTimer alloc] initWithMonitorType:IDbg::MonitorType::kHighCpu];
  [self.highCpuMonitorTimer start];
  
  if (IDbg::MonitorConfigCenter::Instance()->thread_cpu_config_->is_monitor) {
    self.threadCpuMonitorTimer = [[MonitorTimer alloc] initWithMonitorType:IDbg::MonitorType::kThreadCpu];
    [self.threadCpuMonitorTimer start];
  }
  
  BOOL done = NO;
  do {
    CFRunLoopRunResult result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 10, YES);
    if (result == kCFRunLoopRunStopped || result == kCFRunLoopRunFinished) {
      NSLog(@"detect runloop stop");
      done = YES;
    }
    
    if ([[NSThread currentThread] isCancelled] == YES) {
      NSLog(@"detect cancel %d", result);
      done = YES;
    }
    NSLog(@"runloop timer ");
  } while(done == NO);
  NSLog(@"after run");
}

- (void)onStop {
  NSLog(@"real stop thread");
  [self.highCpuMonitorTimer stop];
  [self.threadCpuMonitorTimer stop];
  [self.thread cancel];
  CFRunLoopStop(CFRunLoopGetCurrent());
  NSLog(@"end stop thread");
}

-(void)addAutoSource {
    self.port = [NSMachPort port];
    [self.port setDelegate:self];
    [[NSRunLoop currentRunLoop] addPort:self.port forMode:NSDefaultRunLoopMode];
}

- (void)handlePortMessage:(NSPortMessage *)message {
}

@end
