//
//  LiveVideoCapture.h
//  iPhone_1
//
//  Created by mjzheng on 2020/10/30.
//  Copyright © 2020 mjzheng. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface LiveVideoCapture : NSObject

-(void) initAVCaptureSession:(UIView*) parent;

-(void)start;

-(void)stop;

@end

NS_ASSUME_NONNULL_END
