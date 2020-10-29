//
//  ViewController.m
//  IDbgSample
//
//  Created by mjzheng on 2020/10/10.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#import "ViewController.h"
#import <IDbg/MiniDumpFile.h>
#import <IDbg/ThreadInfo.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}


- (IBAction)onBtnClick:(id)sender {
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
//        MiniDumpFile* file = [[MiniDumpFile alloc] init];
//        NSString* dump = [file generateMiniDump: (DPOptions_STACK | DPOptions_FILES)];
//        NSLog(@"%@", dump);
        
        float appCpu = 0;
        NSArray* ls = getAllThreadBasicInfo(&appCpu);
        for (int i=0; i<ls.count; i++) {
            ThreadInfo* info = (ThreadInfo*)ls[i];
            NSLog(@"thread index %d %@ %@", i, info.name, info.cpu);
        }
    });
}
@end
