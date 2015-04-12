//
//  TicTacHeuristic.h
//  Tests
//
//  Created by Igor on 20.01.15.
//
//

#pragma once

#include "interface.h"

#include "TicTacMemoryBoard.h"

struct TicTacHeuristic : Interface<TicTacHeuristic>
{
  virtual int GetHeuristicScore(TicTacMemoryBoard const& board, Move const& lastMove) const = 0;
};


TicTacHeuristic::Ref CreateDummyHeuristicScore();
TicTacHeuristic::Ref CreateBoardAnalyzerHeuristicScore();
