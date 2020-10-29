//
//  ThreadInfo.h
//  AppleMiniDumpApp
//
//  Created by mjzheng on 2019/6/28.
//  Copyright © 2019年 mjzheng. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN


// (NSString*) getAllThreadStack:(float*) appCpu;

@interface ThreadInfo : NSObject

@property(nonatomic, strong) NSNumber* cpu;
@property(nonatomic, copy) NSString* name;
@property(nonatomic, copy) NSString* stack;

NSString* getAllThreadStack(float* appCpu);

NSArray* getAllThreadBasicInfo(float* appCpu);

@end



NS_ASSUME_NONNULL_END
