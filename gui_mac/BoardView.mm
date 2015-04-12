//
//  BoardView.m
//  TicTacBoardCocoa
//
//  Created by Igor on 09.03.15.
//  Copyright (c) 2015 Igor. All rights reserved.
//

#import "BoardView.h"


@implementation BoardView

/*
-(void)drawRect:(NSRect)dirtyRect {

    NSRect thisViewSize = [self bounds];
  
    int currentSpacing = 6;

    // Set the line color

    [[NSColor colorWithDeviceRed:0 
                           green:(255/255.0) 
                            blue:(255/255.0) 
                           alpha:1] set];

    // Draw the vertical lines first
    NSBezierPath * verticalLinePath = [NSBezierPath bezierPath];

    int gridWidth = thisViewSize.size.width;
    int gridHeight = thisViewSize.size.height;

    int i = 0;

    while (i < gridWidth)
    {
        i = i +  currentSpacing;

        NSPoint startPoint = {i,0};
        NSPoint endPoint = {i, gridHeight};

        [verticalLinePath setLineWidth:1];
        [verticalLinePath moveToPoint:startPoint];
        [verticalLinePath lineToPoint:endPoint];
    }
    [verticalLinePath stroke];
  
    // Draw the horizontal lines

    NSBezierPath * horizontalLinePath = [NSBezierPath bezierPath];

    i = 0;

    while (i < gridHeight)
    {
        i = i + currentSpacing;

        NSPoint startPoint = {0,i};
        NSPoint endPoint = {gridWidth, i};

        [horizontalLinePath setLineWidth:1];
        [horizontalLinePath moveToPoint:startPoint];
        [horizontalLinePath lineToPoint:endPoint];

        [horizontalLinePath stroke];
    }
}
*/

- (void)drawRect:(NSRect)dirtyRect
{
  if (Board)
  {
    NSRect r = [self bounds];
  
    int yp = 0;
    int xp = 0;
    
    int x1, x2, y1, y2;
    Board->GetBoardDimension(x1, y1, x2, y2);
    for (int x = x1; x <= x2; ++x)
    {
      for (int y = y1; y <= y2; y++)
      {
        Side s = Board->GetItemAt(x, y);
        NSImage* im = s == SideX ? ImageX : ImageO;
        //if (s == SideX)
        {
         // [im drawAtPoint:r.origin fromRect:NSZeroRect operation:NSCompositeCopy fraction:1.0];

CGPoint cp;
cp.x = x * 16 + 1 + 16 + 10;
cp.y = y * 16 + 1 + 16 + 10;

          [im drawAtPoint:cp fromRect:NSZeroRect operation:NSCompositeCopy fraction:1.0];
        }
        
        yp += 16 + 1;
        
      }
      xp += 16 + 1;
      yp = 0;
    }
  }
}


-(void) loadResources
{
  ImageX = [NSImage imageNamed:@"ImageX"];
  ImageO = [NSImage imageNamed:@"ImageO"];

}

-(void) setBoard: (TicTacMemoryBoard::Ref) board
{
  Board = board;
  self.needsDisplay = TRUE;
}

@end
