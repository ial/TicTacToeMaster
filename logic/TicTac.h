#pragma once

#include "interface.h"

struct Move
{
  Move(int x, int y)
  : x(x)
  , y(y)
  , invalid()
  {
  }
  bool operator == (Move const &m) const
  {
    return x == m.x && y == m.y;
  }
  
  int x;
  int y;
  bool invalid;
};


/*
 * Hold moves and who played first
 * Can push and pop its state
 */

enum ResultMove
{
  ResultMove_Invalid,
  ResultMove_Win,
  ResultMove_Ok,
};

enum TicTacType
{
  Cross,
  Circle,
  None,
};

enum Side
{
  SideUndefined,
  SideO,
  SideX
};

