//
//  AppDelegate.h
//  TicTacBoardCocoa
//
//  Created by Igor on 07.03.15.
//  Copyright (c) 2015 Igor. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BoardViewController.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (weak) IBOutlet BoardViewController *BoardController;


@end

