//
//  LiveVideoCapture.m
//  iPhone_1
//
//  Created by mjzheng on 2020/10/30.
//  Copyright © 2020 mjzheng. All rights reserved.
//

#import "LiveVideoCapture.h"
#import <AVFoundation/AVFoundation.h>
#include <string>


@interface LiveVideoCapture()<AVCaptureVideoDataOutputSampleBufferDelegate>
@property(nonatomic, strong) AVCaptureSession* session;
@end

@implementation LiveVideoCapture
-(void) initAVCaptureSession:(UIView*) parent {
    self.session = [[AVCaptureSession alloc] init];
    self.session.sessionPreset = AVCaptureSessionPreset1280x720;
    [self.session beginConfiguration];
    
    [self.session commitConfiguration];
    
    [self videoInputAndOutput];
    
    AVCaptureVideoPreviewLayer *previewLayer = [AVCaptureVideoPreviewLayer layerWithSession:self.session];
    previewLayer.frame = parent.bounds;

    [parent.layer addSublayer:previewLayer];
}

-(void)start {
    [self.session startRunning];
}

-(void)stop {
    if ([self.session isRunning]) {
        [self.session stopRunning];
    }
}

-(void) videoInputAndOutput {
    // get device
    AVCaptureDevice* videoDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    
    // set device input
    NSError* error;
    AVCaptureDeviceInput* videoInput =  [[AVCaptureDeviceInput alloc] initWithDevice:videoDevice error:&error];
    if (error != nil) {
        NSLog(@"failed to get %@", error);
    }
    
    if ([self.session canAddInput:videoInput]) {
        [self.session addInput:videoInput];
    }
    
    // set device output
    AVCaptureVideoDataOutput* videoOutput = [[AVCaptureVideoDataOutput alloc] init];
    videoOutput.alwaysDiscardsLateVideoFrames = YES;
    
    NSMutableDictionary* setting = [NSMutableDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange], kCVPixelBufferPixelFormatTypeKey, nil];
    [videoOutput setVideoSettings:setting];
    
    dispatch_queue_t videoQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    [videoOutput setSampleBufferDelegate:self queue:videoQueue];
    
    if ([self.session canAddOutput:videoOutput]) {
        [self.session addOutput:videoOutput];
    }
    
    // now 预览使用GPUImageView
    [self.session setSessionPreset:AVCaptureSessionPreset640x480];
    
    [self setFrameRate:videoDevice AndRate:15];
  
 // [self getCameraDeviceWithPosition:AVCaptureDevicePositionFront];
  
  NSMutableDictionary *formatsDic = [NSMutableDictionary dictionary];
  
  AVCaptureDevicePosition position = AVCaptureDevicePositionFront;
  AVCaptureDevice *captureDevice = [self getCaptureDevicePosition:position];
  
  NSArray<AVCaptureDeviceFormat *> *formats = [captureDevice formats];
  
  for (AVCaptureDeviceFormat *vFormat in formats) {
//      CMFormatDescriptionRef formatDescription = vFormat.formatDescription;
//      NSLog(@"vFormat formatDescription:%@",formatDescription);
//
      CMFormatDescriptionRef description = vFormat.formatDescription;
      CMVideoDimensions dims = CMVideoFormatDescriptionGetDimensions(description);
      NSLog(@"dims width:%d height:%d",dims.width, dims.height);
      
      NSString *key = [NSString stringWithFormat:@"%dx%d",dims.width,dims.height];
      if (![formatsDic objectForKey:key]) {
          [formatsDic setObject:key forKey:key];
      }
  }
}

-(void) audioInputAndOutput {
    
}

// 获取视频帧率
-(int)getCaptureVideoFPS
{
    return captureVideoFPS;
}

// 计算每秒钟采集视频多少帧
static int captureVideoFPS;
- (void)calculatorCaptureFPS
{
    static int count = 0;
    static float lastTime = 0;
    CMClockRef hostClockRef = CMClockGetHostTimeClock();
    CMTime hostTime = CMClockGetTime(hostClockRef);
    float nowTime = CMTimeGetSeconds(hostTime);
    if(nowTime - lastTime >= 1)
    {
        //NSLog(@"capture fps %d", count);
        captureVideoFPS = count;
        lastTime = nowTime;
        count = 0;
    }
    else
    {
        count ++;
    }
}

-(void)setFrameRate: (AVCaptureDevice*) captureDevice AndRate: (int)frameRate
{
    [captureDevice lockForConfiguration:NULL];
    @try {
        [captureDevice setActiveVideoMinFrameDuration:CMTimeMake(1, frameRate)];
        [captureDevice setActiveVideoMaxFrameDuration:CMTimeMake(1, frameRate)];
    } @catch (NSException *exception) {
        NSLog(@"MediaIOS, 设备不支持所设置的分辨率，错误信息：%@",exception.description);
    } @finally {
        
    }
    
    [captureDevice unlockForConfiguration];
}

char*  XXStringForOSType(OSType type) {
    static char c[5];
    c[0] = (type >> 24) & 0xFF;
    c[1] = (type >> 16) & 0xFF;
    c[2] = (type >> 8) & 0xFF;
    c[3] = (type >> 0) & 0xFF;
    c[4] ='\0';
    return c;
}

#pragma mark - AVCaptureVideoDataOutputSampleBufferDelegate
// 获取帧数据
- (void)captureOutput:(AVCaptureOutput *)output didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
    // captureSession 会话如果没有强引用，这里不会得到执行
    
    static NSUInteger count = 0;
    
    CVPixelBufferRef pixelbuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    size_t width = CVPixelBufferGetWidth(pixelbuffer);
    size_t height = CVPixelBufferGetHeight(pixelbuffer);
    OSType result = CVPixelBufferGetPixelFormatType(pixelbuffer);
    count++;
    
   char* ft = XXStringForOSType(result);
    
    //NSLog(@"%u %d %d %s", count, width, height, ft);
    
    [self calculatorCaptureFPS];
    //NSLog(@"----- sampleBuffer ----- %@", sampleBuffer);
}


-(AVCaptureDevice *)getCameraDeviceWithPosition:(AVCaptureDevicePosition )position{
  NSArray *cameras= [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
  for (AVCaptureDevice *camera in cameras) {
    if ([camera position]==position) {
      [[camera formats] enumerateObjectsUsingBlock:^(AVCaptureDeviceFormat* obj, NSUInteger idx, BOOL * _Nonnull stop) {
        
      }];
      return camera;
     }
  }
  return nil;
}


- (AVCaptureDevice *)getCaptureDevicePosition:(AVCaptureDevicePosition)position {
    NSArray *devices = nil;
    if ([UIDevice currentDevice].systemVersion.floatValue >= 10.0) {
        AVCaptureDeviceDiscoverySession *deviceDiscoverySession = [AVCaptureDeviceDiscoverySession discoverySessionWithDeviceTypes:@[AVCaptureDeviceTypeBuiltInWideAngleCamera]
                                                                                                                         mediaType:AVMediaTypeVideo
                                                                                                                          position:position];
        devices = deviceDiscoverySession.devices;
    } else {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
#pragma clang diagnostic pop
    }
    
    for (AVCaptureDevice *device in devices) {
        if (device.position == position) {
            return device;
        }
    }
    return NULL;
}

@end
