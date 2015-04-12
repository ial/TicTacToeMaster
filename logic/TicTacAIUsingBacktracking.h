#pragma once

#include "TicTac.h"

#include "TIcTacMemoryBoard.h"

#include "TicTacBoardVisualizer.h"

#include "TicTacHeuristic.h"

struct TicTacAI : Interface<TicTacAI>
{
  virtual bool SuggestMove(TicTacMemoryBoard const& board, Move& m, int maxDepth) = 0;
  //virtual int GetHeuristicScore(TicTacMemoryBoard const& board) const = 0; //  X over O advance is > 0
};


TicTacAI::Ref CreateTicTacAIUsingBacktracking(TicTacVisualizer::Ref const& visulizer);