//
//  TicTacBoardVisualizer.cpp
//  Tests
//
//  Created by Igor on 10.12.14.
//
//

#include "TicTacBoardVisualizer.h"

#include <iostream>
#include <cmath>

struct DummyTicTacVisualizer : TicTacVisualizer
{
  DummyTicTacVisualizer(bool withPossibleMoves)
   : WithPossibleMoves(withPossibleMoves)
  {
  }

  char GetItem(int coord) const
  {
    coord = abs(coord);
    return coord > 9 ? coord + 'A' - 10 : coord + '0';
  }
  
  virtual void DisplayBoard(TicTacMemoryBoard const& board, Move const& lastMove) const
  {
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    board.GetBoardDimension(x1, y1, x2, y2);
    
    MovesSet_t possibleMoves;
    if (WithPossibleMoves)
    {
      possibleMoves = board.GetAllPossibleMoves();
      --x1;      ++x2;
      --y1;      ++y2;
    }
    
    std::cout << ' ';
    for (int x = x1; x <= x2; ++x)
      std::cout << GetItem(x);
    
    std::cout << std::endl;
    
    for (int y = y1; y <= y2; ++y)
    {
      for (int x = x1; x <= x2; ++x)
      {
        Side s = board.GetItemAt(x, y);
        if (x == x1)
          std::cout << GetItem(y);
      
        Move mv(x, y);
        char symbO = lastMove == mv ? 'O' : 'o';
        char symbX = lastMove == mv ? 'X' : 'x';
        char possible = possibleMoves.count(mv) ? 'p' : ' ';

        std::cout << ( s == SideUndefined ? possible : s == SideX ? symbX : symbO);
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  
  bool WithPossibleMoves;
};

struct EmptyTicTacVisualizer : TicTacVisualizer
{
  virtual void DisplayBoard(TicTacMemoryBoard const& board, Move const& m) const
  {
  }
};

TicTacVisualizer::Ptr CreateDummyTicTacVisualizer(bool withPossibleMoves)
{
  return TicTacVisualizer::Ptr(new DummyTicTacVisualizer(withPossibleMoves));
}

TicTacVisualizer::Ptr CreateEmptyTicTacVisualizer()
{
  return TicTacVisualizer::Ptr(new EmptyTicTacVisualizer());
}