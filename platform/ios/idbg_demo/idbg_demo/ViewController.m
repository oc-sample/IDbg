//
//  ViewController.m
//  idbg_demo
//
//  Created by mjzheng on 2021/4/26.
//

#import "ViewController.h"
#import "IDbg/monitor_thread.h"

@interface ViewController ()

@property (nonatomic, strong) MonitorThread* hb;
@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
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

@end
