//
//  TaskTimer.m
//  iPhone_1
//
//  Created by mjzheng on 2020/10/12.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#import "TaskTimer.h"

@implementation TaskTimer

-(void)startTimer {
    self.timer = [NSTimer timerWithTimeInterval:1 target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
    //[[NSRunLoop currentRunLoop] addTimer:self.timer forMode:NSRunLoopCommonModes]; // 一直有效
    [[NSRunLoop currentRunLoop] addTimer:self.timer forMode:NSDefaultRunLoopMode]; // 默认mode
}

-(void)stopTimer {
    [self.timer invalidate];
    _timer = nil;
}

-(void)onTimer{
    NSLog(@"on timer");
}

@end
