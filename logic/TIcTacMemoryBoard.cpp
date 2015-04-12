
//
//  TIcTacMemoryBoard.c
//  TicTacMaster
//
//  Created by Igor on 06.11.14.
//  Copyright (c) 2014 Igor. All rights reserved.
//

#include "TIcTacMemoryBoard.h"

#include <array>
#include <stdexcept>
#include <vector>
#include <functional>
#include <stdlib.h>
#include <cstdlib>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <memory>

using namespace std::placeholders;

Side GetSide(size_t number);

struct Data
{
  Data()
  : Number(-1)
  {
  }
  
  Side GetSide() const
  {
    if (Number == -1)
      return SideUndefined;
    return ::GetSide(Number);
  }
  size_t Number;
};


typedef std::unordered_map<Move, Data, Hasher> Board_t;

typedef std::unordered_map<Move, MovesSet_t, Hasher> MoveToMoveSet_t;


Side GetSide(size_t number)
{
  return number % 2 ? SideX : SideO;
}

bool HasCommonBorder(Move const& mv, Move const& m)
{
  return std::abs(mv.x - m.x) <= 1 && std::abs(mv.y - m.y) <= 1;
}

int SideWinByDxDy(Board_t const& board, Side s, Move const& m, int dx, int dy)
{
  Move mv(m);
  int i = 0;
  for (; i < 4; ++i)
  {
    mv.x += dx;
    mv.y += dy;
    Board_t::const_iterator iter = board.find(mv);
    if (iter == board.end() || s != iter->second.GetSide())
    {
      break;
    }
  }
  return i;
}

bool SideWinForward(Board_t const& board, Board_t::value_type const& moveData)
{
  Side s = GetSide(board.size());
  if (s != moveData.second.GetSide())
    return false;
  
  if (SideWinByDxDy(board, s, moveData.first, 0, 1) == 4)
    return true;

  if (SideWinByDxDy(board, s, moveData.first, 1, 0) == 4)
    return true;

  if (SideWinByDxDy(board, s, moveData.first, 1, 1) == 4)
    return true;
  
  return false;
}

bool SideWinAll(Board_t const& board, Board_t::value_type const& moveData)
{
  Side s = GetSide(board.size());
  if (s != moveData.second.GetSide())
    return false;
 
  if ((SideWinByDxDy(board, s, moveData.first, 0, -1) + SideWinByDxDy(board, s, moveData.first, 0, 1)) >= 4)
    return true;
  
  if ((SideWinByDxDy(board, s, moveData.first, -1, 0) + SideWinByDxDy(board, s, moveData.first, 1, 0)) >= 4)
    return true;
  
  if ((SideWinByDxDy(board, s, moveData.first, -1, -1) + SideWinByDxDy(board, s, moveData.first, 1, 1)) >= 4)
    return true;

  return false;
}

struct BoardAccess
{
  virtual void AddMove(Move const& mv) = 0;
  virtual bool HasMoves() const = 0;
  virtual bool HasMoveAt(Move const& mv) const = 0;
};

struct TIcTacMemoryBoardSimpleImpl : TicTacMemoryBoard
{
  std::unique_ptr<Board_t> BoardData;
  MovesSet_t CommonBorders; //aka PossibleMoves
  MoveToMoveSet_t ChangesForMove;
  //MoveSet_t UndoCommonBorders; //aka PossibleMoves



  explicit TIcTacMemoryBoardSimpleImpl()
  : BoardData(new Board_t())
  {
  }
  
  explicit TIcTacMemoryBoardSimpleImpl(std::string const& lines)
  : BoardData(new Board_t())
  {
    int numberX = 1;
    int numberO = 2;
    
    int y = 0;
    int x = 0;
    Board_t & Board(*BoardData.get());
    for (std::string::const_iterator iter = lines.begin(); iter != lines.end(); ++iter)
    {
      char ch = *iter;
      if (ch == '\n')
      {
        x = 0;
        ++y;
        continue;
      }
      else if (ch == 'x' || ch == 'X')
      {
        Move m(x, y);
        Data data;
        data.Number = numberX;
        Board[m] = data;
        numberX += 2;
      }
      else if (ch == 'o' || ch == 'O')
      {
        Move m(x, y);
        Data data;
        data.Number = numberO;
        Board[m] = data;
        numberO += 2;
      }
      ++x;
    }
    
    std::for_each(Board.begin(), Board.end(), std::bind(&TIcTacMemoryBoardSimpleImpl::AccumulatePossibleMoves, this, std::ref(CommonBorders), std::bind(&Board_t::value_type::first, _1)));
  }
  
  explicit TIcTacMemoryBoardSimpleImpl(TIcTacMemoryBoardSimpleImpl const& obj)
  : BoardData(new Board_t(*obj.BoardData.get()))
  , CommonBorders(obj.CommonBorders)
  , ChangesForMove(obj.ChangesForMove)
  {
  }
  
  void AddCommonBorders(Move const& m)
  {
    Board_t const& Board(*BoardData.get());
    CommonBorders.erase(m);
    MovesSet_t addedPossibleMoves;
    for (int dx = -1; dx <= 1; ++dx)
    {
      for (int dy = -1; dy <= 1; ++dy)
      {
        if (dx == 0 && dy == 0)
          continue;
        
        Move mv(m.x + dx, m.y + dy);
        Board_t::const_iterator iter = Board.find(mv);
        if (iter == Board.end())
        {
          std::pair<MovesSet_t::const_iterator, bool> moveInserted = CommonBorders.insert(mv);
          if (moveInserted.second)
          {
            addedPossibleMoves.insert(mv);
          }
        }
      }
    }
    ChangesForMove[m] = addedPossibleMoves;
  }
  
  void RemoveCommonBorders(Move const& m)
  {
    MoveToMoveSet_t::iterator iter = ChangesForMove.find(m);
    if (iter != ChangesForMove.end())
    {
      MovesSet_t const& addedPossibleMoves = iter->second;
      //CommonBorders.insert(addedPossibleMoves.begin(), addedPossibleMoves.end());
      //CommonBorders.era
/*
      MoveSet_t& mset = CommonBorders;
      std::for_each(addedPossibleMoves.begin(), addedPossibleMoves.end(), [mset](Move const& mv) mutable
      {
        mset.erase(mv);
      });
*/
      for (MovesSet_t::const_iterator iter = addedPossibleMoves.begin(); iter != addedPossibleMoves.end(); ++iter)
      {
        CommonBorders.erase(*iter);
      }

      CommonBorders.insert(m);
      ChangesForMove.erase(iter);
    }
  }
  
  bool CanMove(Move const& mv) const
  {
    Board_t const& Board(*BoardData.get());
    if (Board.empty())
      return true;
    
    return CommonBorders.find(mv) != CommonBorders.end();
 /*
    if (std::any_of(Board.begin(), Board.end(), [mv](Board_t::value_type const& _val) {return _val.first == mv;})) //std::bind(&Board_t::value_type::first, _1) == 
      return false;
    

    if (std::none_of(Board.begin(), Board.end(), [mv](Board_t::value_type const& _val) {return HasCommonBorder(mv, _val.first);}))
                     //std::bind(&HasCommonBorder, [mv](Board_t::value_type const& _val) {return _val.first == mv;})))
      return false;
  */
    return true;
  }
  
  bool Finished() const
  {
    Board_t const& Board(*BoardData.get());
    return std::any_of(Board.begin(), Board.end(), std::bind(&SideWinForward, Board, _1));
  }
  
  bool Finished(Move const& m) const
  {
    Board_t const& Board(*BoardData.get());
    Board_t::const_iterator iter = Board.find(m);
    if (iter != Board.end())
    {
      return SideWinAll(Board, *iter);
    }
    return false;
  }
  
  virtual ResultMove MakeMove(Move const& m)
  {
    Board_t& Board(*BoardData.get());
    if (!CanMove(m))
      return ResultMove_Invalid;
    
    Data data;
    data.Number = Board.size() + 1;
    Board[m] = data;
    
    AddCommonBorders(m);

    if (Finished(m))
      return ResultMove_Win;
    
    return ResultMove_Ok;
  }

  void AccumulatePossibleMoves(MovesSet_t& moves, Move const& m) const
  {
    Board_t const& Board(*BoardData.get());
    for (int dx = -1; dx <= 1; ++dx)
    {
      for (int dy = -1; dy <= 1; ++dy)
      {
        if (dx == 0 && dy == 0)
          continue;
        
        Move mv(m.x + dx, m.y + dy);
        if (moves.find(mv) == moves.end())
        {
          Board_t::const_iterator iter = Board.find(mv);
          if (iter == Board.end())
          {
            moves.insert(mv);
          }
        }
      }
    }
  }
  
  virtual MovesSet_t GetAllPossibleMoves() const
  {
    Board_t const& Board(*BoardData.get());
    MovesSet_t moves;
    if (Board.empty())
    {
      moves.insert(Move(0,0));
      return moves;
    }
    /*
    else
    {
      std::for_each(Board.begin(), Board.end(), std::bind(&TIcTacMemoryBoardSimpleImpl::AccumulatePossibleMoves, this, std::ref(moves), std::bind(&Board_t::value_type::first, _1)));
    }
    */
    return CommonBorders;
  }

  virtual void UndoMove(Move const& mv)
  {
    Board_t& Board(*BoardData.get());
    Board.erase(mv);
    RemoveCommonBorders(mv);
  }
  
  virtual Side GetItemAt(int x, int y) const
  {
    Board_t const& Board(*BoardData.get());
    Move mv(x, y);
    Board_t::const_iterator iter = Board.find(mv);
    if (iter != Board.end())
    {
      return iter->second.GetSide();
    }
    return SideUndefined;
  }

  virtual void GetBoardDimension(int& x1, int& y1, int& x2, int& y2) const
  {
    Board_t const& Board(*BoardData.get());
    x1 = 0; x2 = 0; y1 = 0; y2 = 0;

    auto resultX = std::minmax_element(std::begin(Board), std::end(Board),
                                       [](Board_t::value_type const& _boardData1, Board_t::value_type const& _boardData2)
                                       {
                                         return _boardData1.first.x < _boardData2.first.x;
                                       });
  
    auto resultY = std::minmax_element(std::begin(Board), std::end(Board),
                                       [](Board_t::value_type const& _boardData1, Board_t::value_type const& _boardData2)
                                       {
                                         return _boardData1.first.y < _boardData2.first.y;
                                       });
    
    if (resultX.first != std::end(Board))
    {
      x1 = resultX.first->first.x;
    }
    if (resultX.second != std::end(Board))
    {
      x2 = resultX.second->first.x;
    }
    if (resultY.first != std::end(Board))
    {
      y1 = resultY.first->first.y;
    }
    if (resultY.second != std::end(Board))
    {
      y2 = resultY.second->first.y;
    }
  }
  
  virtual size_t GetHash() const
  {
    Board_t const& Board(*BoardData.get());
    size_t hash = 0;
    std::for_each(Board.begin(), Board.end(), [&](Board_t::value_type const& boardData)
    {
      Move const& m = boardData.first;
      hash = (hash + (324723947 + Hasher()(m)) ^93485734985); // http://stackoverflow.com/questions/2285822/c-what-is-a-good-way-to-hash-array-data
    });
    return hash;
  }
  
  virtual TicTacMemoryBoard::Ref Clone() const
  {
    return TicTacMemoryBoard::Ref(new TIcTacMemoryBoardSimpleImpl(*this));
  }

};

TicTacMemoryBoard::Ref CreateTIcTacMemoryBoardSimple()
{
  return TicTacMemoryBoard::Ref(new TIcTacMemoryBoardSimpleImpl());
}

TicTacMemoryBoard::Ref CreateTIcTacMemoryBoardSimple(std::string const& lines)
{
  return TicTacMemoryBoard::Ref(new TIcTacMemoryBoardSimpleImpl(lines));
}

