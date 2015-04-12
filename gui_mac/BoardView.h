//
//  BoardView.h
//  TicTacBoardCocoa
//
//  Created by Igor on 09.03.15.
//  Copyright (c) 2015 Igor. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "logic/TicTacMemoryBoard.h"

@interface BoardView : NSView
{
  NSImage* ImageX;
  NSImage* ImageO;
  TicTacMemoryBoard::Ref Board;
}

-(void) loadResources;
-(void) setBoard: (TicTacMemoryBoard::Ref) Board;

@end
