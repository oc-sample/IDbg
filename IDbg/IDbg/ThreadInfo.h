//
//  ThreadInfo.h
//  AppleMiniDumpApp
//
//  Created by mjzheng on 2019/6/28.
//  Copyright © 2019年 mjzheng. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <mach/mach.h>

NS_ASSUME_NONNULL_BEGIN


// (NSString*) getAllThreadStack:(float*) appCpu;

@interface ThreadInfo : NSObject

@property(nonatomic, strong) NSNumber* cpu;
@property(nonatomic, copy) NSString* name;
@property(nonatomic, copy) NSString* stack;
//@property(nonatomic, assign) thread_t th;
@property(nonatomic, assign)NSNumber* th;

NSString* getAllThreadStack(float* appCpu);

NSArray* getAllThreadBasicInfo(float* appCpu);

NSString* getAllThreadStr();

float getSysCpu();

@end

NS_ASSUME_NONNULL_END
