//
//  AppDelegate.m
//  TicTacBoardCocoa
//
//  Created by Igor on 07.03.15.
//  Copyright (c) 2015 Igor. All rights reserved.
//

#import "AppDelegate.h"

#include "logic/TIcTacMemoryBoard.h"

#include <string>

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{

  std::string lines =
    "ooo\n"
    "xxx\n"
    "xox\n";
  
  TicTacMemoryBoard::Ref board = CreateTIcTacMemoryBoardSimple(lines);
 // _BoardController->star
 [_BoardController startFromBoard: board];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
  // Insert code here to tear down your application
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    return YES;
}

@end
