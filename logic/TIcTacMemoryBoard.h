//
//  TIcTacMemoryBoard.h
//  TicTacMaster
//
//  Created by Igor on 06.11.14.
//  Copyright (c) 2014 Igor. All rights reserved.
//

#pragma once

#include "TicTac.h"

#include <array>
#include <unordered_set>
#include <vector>
#include <string>
//#include <set>

//typedef std::set<Move, Comparer> MovesSet_t;
struct Hasher;

typedef std::unordered_set<Move, Hasher> MovesSet_t;
typedef std::array<MovesSet_t, 2> MoveSetByPlayers;

struct Hasher
{
  inline size_t operator() (Move const& mv) const
  {
    return (mv.x << 16) + mv.y;
  }
  
  inline size_t operator() (MovesSet_t const& mv) const
  {
    size_t result = 0;
    for(MovesSet_t::const_iterator iter = mv.begin(); iter != mv.end(); ++iter)
    {
      result ^= operator()(*iter);
    }
    return result;
  }
  
  inline size_t operator() (MoveSetByPlayers const& movesByPLayer) const
  {
    size_t result = 0;
    for(MoveSetByPlayers::const_iterator iter = movesByPLayer.begin(); iter != movesByPLayer.end(); ++iter)
    {
      result ^= operator()(*iter);
    }
    return result;
  }
};

struct Equal_to
{
  bool operator() (const MovesSet_t& x, const MovesSet_t& y) const
  {
    return x==y;
  }
};

struct Comparer
{
  inline bool operator() (Move const& l, Move const&r) const
  {
    return l.x == r.x ? l.y < r.y : l.x < r.x;
  }
};


struct BoardSimple
{
  virtual Side GetItemAt(int x, int y) const = 0;
};

struct TicTacMemoryBoard : Interface<TicTacMemoryBoard>
{
  virtual bool CanMove(Move const& mv) const = 0;
  virtual bool Finished() const = 0;
  virtual MovesSet_t GetAllPossibleMoves() const = 0;
  virtual void GetBoardDimension(int& x1, int& y1, int& x2, int& y2) const = 0;
  virtual Side GetItemAt(int x, int y) const = 0;
  virtual TicTacMemoryBoard::Ref Clone() const = 0;
  virtual ResultMove MakeMove(Move const& mv) = 0;
  virtual void UndoMove(Move const& mv) = 0;
  virtual size_t GetHash() const = 0;
};

TicTacMemoryBoard::Ref CreateTIcTacMemoryBoardSimple();
TicTacMemoryBoard::Ref CreateTIcTacMemoryBoardSimple(std::string const& lines);
//TicTacActiveBoard::Ref CreateActiveBoard(TicTacMemoryBoard& board);
