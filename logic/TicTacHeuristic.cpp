//
//  TicTacHeuristic.h
//  Tests
//
//  Created by Igor on 20.01.15.
//
//

#include "TicTacHeuristic.h"

#include "TIcTacMemoryBoard.h"

struct DummyTicTacHeuristic : TicTacHeuristic
{
  virtual int GetHeuristicScore(TicTacMemoryBoard const& board, Move const& lastMove) const
  {
    return 0;
  }
};

TicTacHeuristic::Ref CreateDummyHeuristicScore()
{
  return TicTacHeuristic::Ref();
}

TicTacHeuristic::Ref CreateBoardAnalyzerHeuristicScore()
{
  return TicTacHeuristic::Ref();
}