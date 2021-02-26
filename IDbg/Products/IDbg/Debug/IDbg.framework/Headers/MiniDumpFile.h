//
//  MiniDumpFile.h
//  AppleMiniDumpApp
//
//  Created by mjzheng on 2019/6/28.
//  Copyright © 2019年 mjzheng. All rights reserved.
//

#import <Foundation/Foundation.h>

// lipo -create Release-iphoneos/libThreadSnapshot.a Release-iphonesimulator/libThreadSnapshot.a -output libThreadSnapshot.a

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, DPOptions) {
    DPOptions_HEADER           =  1 << 0,   // 0000 0001
    DPOptions_STACK            =  1 << 1,   // 0000 0010
    DPOptions_IMAGE            =  1 << 2,   // 0000 0100
    DPOptions_FILES            =  1 << 3,   // 0000 1000
};


@interface MiniDumpFile : NSObject

-(NSString*) generateMiniDump:(DPOptions) options;

-(NSArray*) getAllThreadBasicInfo;

-(instancetype) init;

@end

NS_ASSUME_NONNULL_END
