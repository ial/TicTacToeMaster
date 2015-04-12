//
//  BoardViewController.m
//  TicTacBoardCocoa
//
//  Created by Igor on 17.03.15.
//  Copyright (c) 2015 Igor. All rights reserved.
//

#import "BoardViewController.h"
#import "BoardView.h"

#include "logic/TIcTacMemoryBoard.h"

#include <string>

@interface BoardViewController ()

@end

@implementation BoardViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do view setup here.
}

-(void) startFromBoard: (TicTacMemoryBoard::Ref const&)board
{
  Board = board->Clone();
  BoardView* bw = self.view;
  [bw loadResources];
  [bw setBoard: board];
//  [self.contentView setBoard: board];
}

@end
