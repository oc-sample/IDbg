//
//  ViewController.m
//  idbg_demo
//
//  Created by mjzheng on 2021/4/26.
//

#import "ViewController.h"
#import "IDbg/monitor_thread.h"
#import "av/LiveVideoCapture.h"

@interface ViewController ()

@property (nonatomic, strong) MonitorThread* hb;
@property(nonatomic, strong) LiveVideoCapture* videoCaputre;
@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  self.videoCaputre = [[LiveVideoCapture alloc] init];
  [self.videoCaputre initAVCaptureSession:self.videoView];
  // Do any additional setup after loading the view.
}

- (void)stopHeartbeat {
  [self.hb stop];
}

- (void)startHeartbeat{
  self.hb = [[MonitorThread alloc] init];
  [self.hb start];
}

- (IBAction)onStart:(id)sender {
  [self startHeartbeat];
}

- (IBAction)onStop:(id)sender {
  [self stopHeartbeat];
}

- (IBAction)onStartVideo:(id)sender {
  [self.videoCaputre start];
}

- (IBAction)onStopVideo:(id)sender {
  [self.videoCaputre stop];
}

@end
