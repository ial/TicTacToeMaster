//
//  BoardViewController.h
//  TicTacBoardCocoa
//
//  Created by Igor on 17.03.15.
//  Copyright (c) 2015 Igor. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "logic/TIcTacMemoryBoard.h"

@interface BoardViewController : NSViewController
{
  TicTacMemoryBoard::Ref Board;
}

-(void) startFromBoard: (TicTacMemoryBoard::Ref const&) Board;
@end
