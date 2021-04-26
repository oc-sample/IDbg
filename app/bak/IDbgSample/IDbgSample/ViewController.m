//
//  ViewController.m
//  IDbgSample
//
//  Created by mjzheng on 2020/10/10.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#import "ViewController.h"
#import "Heartbeat.h"

@interface ViewController ()

@property (nonatomic, strong) Heartbeat* hb;


@end

@implementation ViewController

- (void)viewDidLoad {
    [[NSThread currentThread] setName:@"main"];
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)stopHeartbeat {
  [self.hb stop];
}

- (void)startHeartbeat{
  self.hb = [[Heartbeat alloc] init];
  [self.hb start];
}

- (IBAction)onStop:(id)sender {
  [self stopHeartbeat];
  //NSLog(@"onStop");
}

- (IBAction)onBtnClick:(id)sender {
  [self startHeartbeat];
}

//dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
//        MiniDumpFile* file = [[MiniDumpFile alloc] init];
//        NSString* dump = [file generateMiniDump: (DPOptions_STACK | DPOptions_FILES)];
//        NSLog(@"%@", dump);
    
//        float appCpu = 0;
//        NSArray* ls = getAllThreadBasicInfo(&appCpu);
//        NSMutableString* header = [[NSMutableString alloc] init];
//        [header appendString:@"sys app "];
//        for (int i=0; i<ls.count; i++) {
//            ThreadInfo* info = (ThreadInfo*)ls[i];
//            NSString* threadName = @"";
//            if (info != nil) {
//                if (info.name != nil && ![info.name isEqual:@""]) {
//                    threadName = info.name;
//                } else {
//                    threadName = [NSString stringWithFormat:@"%lu", [info.th longValue]];
//                }
//            }
//            [header appendFormat:@"%@ ", threadName];
//        }
//        NSLog(@"header %@", header);
//
//        NSMutableString* line = [[NSMutableString alloc] init];
//        for (int i=0; i<ls.count; i++) {
//            ThreadInfo* info = (ThreadInfo*)ls[i];
//            [line appendFormat:@"%@ ", info.cpu];
//        }
//
//        float sysCpu = getSysCpu();
//        NSLog(@"line %0.2f %0.2f %@", sysCpu, appCpu, line);
//  NSLog(@"%@", getAllThreadStr());
//});
@end
