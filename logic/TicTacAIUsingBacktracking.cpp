//
//  TicTacAIUsingBacktracking.cpp
//  TicTacMaster
//
//  Created by Igor on 23.11.14.
//  Copyright (c) 2014 Igor. All rights reserved.
//

#include "TicTacAIUsingBacktracking.h"

#include <array>
#include <assert.h>
#include <vector>
#include <list>
#include <set>
#include <iterator>
#include <unordered_map>

typedef std::list<Move> MoveSequence_t;
typedef std::pair<int, MoveSequence_t> ScoreMoveSequence_t;

typedef std::unordered_map<MoveSetByPlayers, ScoreMoveSequence_t, Hasher> MapMovesSetToScoreMoveSequence_t;
//typedef std::unordered_map<int, int> MapMovesSetToScoreMoveSequence_t;

unsigned PlayersCount = 2;

struct ScoreDirection
{
  virtual ~ScoreDirection() {}
  virtual int GetWinScore(int depth) const = 0;
  virtual int GetInitScore() const = 0;
  virtual bool IsAccepted(ScoreMoveSequence_t const& newScore, ScoreMoveSequence_t const& bestScore) const = 0;
  virtual int GetHeuristicScore(TicTacMemoryBoard const& board, Move const& mv, int dept) const = 0;
  virtual ScoreDirection const& GetInverted() const = 0;
};

struct MinimazingPlayer : ScoreDirection
{
  virtual int GetWinScore(int depth) const;
  virtual int GetInitScore() const;
  virtual bool IsAccepted(ScoreMoveSequence_t const& newScore, ScoreMoveSequence_t const& bestScore) const;
  virtual int GetHeuristicScore(TicTacMemoryBoard const& board, Move const& mv, int depth) const;
  virtual ScoreDirection const& GetInverted() const;
};

struct MaximizingPlayer : ScoreDirection
{
  virtual int GetWinScore(int depth) const;
  virtual int GetInitScore() const;
  virtual bool IsAccepted(ScoreMoveSequence_t const& newScore, ScoreMoveSequence_t const& bestScore) const;
  virtual int GetHeuristicScore(TicTacMemoryBoard const& board, Move const& mv, int depth) const;
  virtual ScoreDirection const& GetInverted() const;
};

static MaximizingPlayer maxPlayerDirection;
static MinimazingPlayer minPlayerDirection;

int MaximizingPlayer::GetWinScore(int depth) const
{
  return 1000 - depth;
}
int MaximizingPlayer::GetInitScore() const
{
  return -100000;
}
bool MaximizingPlayer::IsAccepted(ScoreMoveSequence_t const& newScore, ScoreMoveSequence_t const& bestScore) const
{
  return newScore.first > bestScore.first;
}

namespace
{
  int SideWinByDxDy(TicTacMemoryBoard const& board, Side s, Move const& m, int dx, int dy)
  {
    int sum = 0;
    Move mv(m);
    int i = 0;
    for (; i < 4; ++i)
    {
      mv.x += dx;
      mv.y += dy;
      
      Side is = board.GetItemAt(mv.x, mv.y);
      if (is == SideUndefined)
      {
        sum += 1;
      }
      else if (is == s)
      {
        sum += 5;
      }
      else
      {
        return sum;
      }
    }
    return sum;
  }

  int SideWinAll(TicTacMemoryBoard const& board, Move const& m, int depth)
  {
    //Side s = board.GetItemAt(m.x, m.y);
    return 0;
      //+ SideWinByDxDy(board, s, m, 0, 1) + SideWinByDxDy(board, s, m, 0, -1)
      //+ SideWinByDxDy(board, s, m, 1, 0) + SideWinByDxDy(board, s, m, -1, 0)
      //+ SideWinByDxDy(board, s, m, 1, 1) + SideWinByDxDy(board, s, m, -1, -1);
  }
}

int MaximizingPlayer::GetHeuristicScore(TicTacMemoryBoard const& board, Move const& mv, int depth) const
{
  return ::SideWinAll(board, mv, depth);
}

ScoreDirection const& MaximizingPlayer::GetInverted() const
{
  return minPlayerDirection;
}


int MinimazingPlayer::GetWinScore(int depth) const
{
  return -MaximizingPlayer().GetWinScore(depth);
}
int MinimazingPlayer::GetInitScore() const
{
  return -MaximizingPlayer().GetInitScore();
}
bool MinimazingPlayer::IsAccepted(ScoreMoveSequence_t const& newScore, ScoreMoveSequence_t const& bestScore) const
{
  return !MaximizingPlayer().IsAccepted(newScore, bestScore);
}
int MinimazingPlayer::GetHeuristicScore(TicTacMemoryBoard const& board, Move const& mv, int depth) const
{
  return -MaximizingPlayer().GetHeuristicScore(board, mv, depth);
}
ScoreDirection const& MinimazingPlayer::GetInverted() const
{
  return maxPlayerDirection;
}


struct TicTacAIUsingBacktracking : public TicTacAI
{
  TicTacAIUsingBacktracking(TicTacVisualizer::Ref const& visualizer)
    : Visualizer(visualizer)
  {
  }

  ScoreMoveSequence_t minimax(TicTacMemoryBoard& board, int depth, ScoreDirection const& direction, MapMovesSetToScoreMoveSequence_t& madeMovesSet, MoveSetByPlayers& madeMoves)
  {
    MovesSet_t moves = board.GetAllPossibleMoves();

    ScoreMoveSequence_t best = std::make_pair(direction.GetInitScore(), MoveSequence_t());
    for (MovesSet_t::const_iterator iter = moves.begin(); iter != moves.end(); ++iter)
    {
      Move const& m(*iter);
      
      std::pair<MovesSet_t::const_iterator, bool> madeMove = madeMoves[depth % PlayersCount].insert(m);
      assert(madeMove.second);
      
      ScoreMoveSequence_t current = std::make_pair(direction.GetInitScore(), MoveSequence_t());
      bool final = false;
      MapMovesSetToScoreMoveSequence_t::const_iterator madeMovesIter = madeMovesSet.find(madeMoves);
      if (madeMovesIter == madeMovesSet.end())
      {
        ResultMove result = board.MakeMove(m);
        
//        Visualizer->DisplayBoard(board, m);
        
        assert(result != ResultMove_Invalid);
        
        if (result == ResultMove_Win)
        {
          current.first = direction.GetWinScore(depth);
          current.second.push_front(m);
          final = true;
        }
        else if (depth == 1)
        {
          current.first = direction.GetHeuristicScore(board, m, depth);
          current.second.push_front(m);
        }
        else
        {
          current = minimax(board, depth - 1, direction.GetInverted(), madeMovesSet, madeMoves);
          if (!current.second.empty())
            current.second.push_front(m);
        }
        
        board.UndoMove(m);
        
#ifdef _DEBUG
        ScoreMoveSequence_t ff;
        if (madeMovesIter != madeMovesSet.end())
        {
          ff = madeMovesIter->second;
          if (ff.first != current.first)
          {
            abort();
          }
        }
#endif
        madeMovesSet[madeMoves] = current;
      }
      else
      {
        current = madeMovesIter->second;
      }
      madeMoves[depth % PlayersCount].erase(madeMove.first);
      
      if (final)
      {
        return current;
      }
      
      if (!current.second.empty() && (best.second.empty() || direction.IsAccepted(current, best)))
      {
        best = current;
      }
    }
    return best;
  }
  
  
  virtual bool SuggestMove(TicTacMemoryBoard const& board, Move& mv, int maxDepth)
  {
    TicTacMemoryBoard::Ref boardCopy(board.Clone());
    MapMovesSetToScoreMoveSequence_t madeMovesSet;
    MoveSetByPlayers madeMoves;
    ScoreMoveSequence_t result = minimax(*boardCopy.get(), maxDepth, maxPlayerDirection, madeMovesSet, madeMoves);
    if (result.second.size())
    {
      mv = result.second.front();
      return true;
    }
    return false;
  }
  
  TicTacVisualizer::Ref Visualizer;
};


TicTacAI::Ref CreateTicTacAIUsingBacktracking(TicTacVisualizer::Ref const& visualizer)
{
  return TicTacAI::Ref(new TicTacAIUsingBacktracking(visualizer));
}