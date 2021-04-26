//
//  ViewController.m
//  idbg_demo
//
//  Created by mjzheng on 2021/4/26.
//

#import "ViewController.h"
#import <IDbg/mini_dump_file.h>

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];

  // Do any additional setup after loading the view.
}


- (void)setRepresentedObject:(id)representedObject {
  [super setRepresentedObject:representedObject];

  // Update the view, if already loaded.
}

- (IBAction)onClickButton:(id)sender {
  std::string file = IDbg::GenerateMiniDump();
  NSLog(@"on start %@", [NSString stringWithUTF8String:file.c_str()]);
}

@end
