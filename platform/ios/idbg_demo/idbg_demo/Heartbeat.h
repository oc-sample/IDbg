//
//  Heartbeat.h
//  iPhone_1
//
//  Created by mjzheng on 2020/10/12.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface Heartbeat : NSObject

@property(nonatomic, strong) NSThread* thread;

-(void)start;

-(void)stop;


@end

NS_ASSUME_NONNULL_END