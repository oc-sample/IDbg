//
//  ViewController.m
//  IDbgPods
//
//  Created by mjzheng on 2020/10/10.
//  Copyright © 2020年 mjzheng. All rights reserved.
//

#import "ViewController.h"

#import "MiniDumpFile.h"
#import "AFNetworking.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}


- (IBAction)onBtnClick:(id)sender {
    MiniDumpFile* file = [[MiniDumpFile alloc] init];
    NSString* dump = [file generateMiniDump];
    NSLog(@"%@", dump);
}


@end
