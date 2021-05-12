//
//  Heartbeat.m
//  iPhone_1
//
//  Created by mjzheng on 2020/10/12.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#import "heartbeat.h"
#import "mini_dump_file.h"

@interface Heartbeat()
@property(nonatomic, strong) NSThread* thread;
@property(nonatomic, strong)NSTimer* timer;

@end

@implementation Heartbeat

- (instancetype)init {
  self = [super init];
  if (self) {
    
  }
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
    [self.thread cancel];
    [self performSelector:@selector(onStop) onThread:self.thread withObject:nil waitUntilDone:YES];
    _thread = nil;
  }
}

- (void)run {
  NSLog(@"before run");
  [self startTimer];
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
  [self stopTimer];
  CFRunLoopStop(CFRunLoopGetCurrent());
  NSLog(@"end stop thread");
}

- (void)startTimer {
  self.timer = [NSTimer timerWithTimeInterval:1 target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
  [[NSRunLoop currentRunLoop] addTimer:self.timer forMode:NSRunLoopCommonModes]; // 一直有效
    //[[NSRunLoop currentRunLoop] addTimer:self.timer forMode:NSDefaultRunLoopMode]; // 默认mode
}

- (void)stopTimer {
  [self.timer invalidate];
  _timer = nil;
}

- (void)onTimer{
//  std::string file = IDbg::GenerateMiniDump(IDbg::kDefault);
//
//  IDbg::CreateFileDirectories();
//
//  IDbg::SaveToFile(file);
//  NSLog(@"%@", [NSString stringWithUTF8String:file.c_str()]);
}

@end
