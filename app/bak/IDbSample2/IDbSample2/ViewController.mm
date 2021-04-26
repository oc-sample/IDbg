//
//  ViewController.m
//  IDbSample2
//
//  Created by mjzheng on 2021/3/11.
//

#import "ViewController.h"
#import <IDbg/ThreadInfo.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
  ThreadStackList ls;
  getThreadStackList(ls);
  [super viewDidLoad];
  // Do any additional setup after loading the view.
}


@end
