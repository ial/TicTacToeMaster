#pragma once

#include "TicTacMemoryBoard.h"
#include "Interface.h"


struct TicTacVisualizer : Interface<TicTacVisualizer>
{
  virtual void DisplayBoard(TicTacMemoryBoard const& board, Move const& mv) const = 0;
};

TicTacVisualizer::Ptr CreateDummyTicTacVisualizer(bool possibleMoves = false);
TicTacVisualizer::Ptr CreateEmptyTicTacVisualizer();