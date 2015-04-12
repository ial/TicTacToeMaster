
#include <logic/TIcTacMemoryBoard.h>
#include <logic/TIcTacAIUsingBacktracking.h>
#include <logic/TicTacBoardVisualizer.h>

#include <gtest/gtest.h>

struct TicTacMasterTests : ::testing::Test
{
  virtual void SetUp()
  {
    ::testing::Test::SetUp();
    Board = CreateTIcTacMemoryBoardSimple();
    AI = CreateTicTacAIUsingBacktracking(
                                         //CreateDummyTicTacVisualizer()
                                         CreateEmptyTicTacVisualizer()
                                         );
  }

  virtual void TearDown()
  {
    ::testing::Test::TearDown();
  }

  TicTacMemoryBoard::Ref Board;
  TicTacAI::Ref AI;
};



TEST_F(TicTacMasterTests, Scenario3X)
{
  std::string board =
    "ooo\n"
    "xxx\n";
  
  Board = CreateTIcTacMemoryBoardSimple(board);
 // CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get(), Move(0,0));
  
  Move m(0,0);
  EXPECT_TRUE(AI->SuggestMove(*Board.get(), m, 4));
  ResultMove rm = Board->MakeMove(m);
  EXPECT_NE(rm, ResultMove_Invalid);
  
  CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get(), m);
  EXPECT_TRUE(m == Move(-1, 1) || m == Move(3, 1));
}

TEST_F(TicTacMasterTests, Scenario3O)
{
  std::string board =
    "ooo\n"
    "xxx\n"
    "x\n";
  
  Board = CreateTIcTacMemoryBoardSimple(board);
 // CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get(), Move(0,0));
  
  Move m(0,0);
  EXPECT_TRUE(AI->SuggestMove(*Board.get(), m, 4));
  ResultMove rm = Board->MakeMove(m);
  EXPECT_NE(rm, ResultMove_Invalid);
  
  CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get(), m);
  EXPECT_TRUE(m == Move(-1, 0) || m == Move(3, 0));
}

TEST_F(TicTacMasterTests, testValidMoves)
{
   EXPECT_TRUE(Board->MakeMove(Move(0, 0)) == ResultMove_Ok);

   EXPECT_TRUE(Board->CanMove(Move(-1, -1)));
   EXPECT_TRUE(Board->CanMove(Move(0, -1)));
   EXPECT_TRUE(Board->CanMove(Move(1, -1)));

   EXPECT_TRUE(Board->CanMove(Move(-1, 0)));
   EXPECT_TRUE(Board->CanMove(Move(1, 0)));

   EXPECT_TRUE(Board->CanMove(Move(-1, 1)));
   EXPECT_TRUE(Board->CanMove(Move(0, 1)));
   EXPECT_TRUE(Board->CanMove(Move(1, 1)));


   EXPECT_FALSE(Board->CanMove(Move(-2, -2)));
   EXPECT_FALSE(Board->CanMove(Move(0, -2)));
   EXPECT_FALSE(Board->CanMove(Move(2, -2)));

   EXPECT_FALSE(Board->CanMove(Move(-2, 0)));
   EXPECT_FALSE(Board->CanMove(Move(2, 0)));

   EXPECT_FALSE(Board->CanMove(Move(-2, 2)));
   EXPECT_FALSE(Board->CanMove(Move(0, 2)));
   EXPECT_FALSE(Board->CanMove(Move(2, 2)));
}


TEST_F(TicTacMasterTests, testGetItemAt)
{
  EXPECT_EQ(Board->GetItemAt(0, 0), SideUndefined);
  EXPECT_EQ(Board->MakeMove(Move(0, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->GetItemAt(0, 0), SideX);
  EXPECT_EQ(Board->MakeMove(Move(1, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->GetItemAt(1, 0), SideO);
}

TEST_F(TicTacMasterTests, testXWinsVertSimple)
{
  EXPECT_EQ(Board->MakeMove(Move(0, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(0, 0)), ResultMove_Invalid);
  EXPECT_EQ(Board->MakeMove(Move(0, 1)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(1, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(1, 1)), ResultMove_Ok);
//CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get());
  EXPECT_EQ(Board->MakeMove(Move(2, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(2, 1)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(3, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(3, 1)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(4, 0)), ResultMove_Win);
}


TEST_F(TicTacMasterTests, testClone)
{
  EXPECT_EQ(Board->MakeMove(Move(0, 0)), ResultMove_Ok);
  TicTacMemoryBoard::Ref boardCopy = Board->Clone();
  EXPECT_EQ(Board->GetAllPossibleMoves().size(), boardCopy->GetAllPossibleMoves().size());
  EXPECT_EQ(Board->GetHash(), boardCopy->GetHash());
}

/*
TEST_F(TicTacMasterTests, testOPreventsXWinVertSimple)
{
  EXPECT_EQ(Board->MakeMove(Move(0, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(0, 0)), ResultMove_Invalid);
  EXPECT_EQ(Board->MakeMove(Move(0, 1)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(1, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(1, 1)), ResultMove_Ok);
  //
  EXPECT_EQ(Board->MakeMove(Move(2, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(2, 1)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(3, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(3, 2)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(4, 2)), ResultMove_Ok);
  CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get());
  Move m(0,0);
  EXPECT_TRUE(AI->SuggestMove(*Board.get(), m, 2));
  EXPECT_TRUE(m == Move(4, 0) || m == Move(-1, 0));
  EXPECT_EQ(Board->MakeMove(m), ResultMove_Ok);
  CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get());
}
 */

TEST_F(TicTacMasterTests, testXWinsVertByAI)
{
  EXPECT_EQ(Board->MakeMove(Move(0, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(0, 1)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(1, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(1, 1)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(2, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(2, 1)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(3, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(3, 1)), ResultMove_Ok);
  
  Move m(0,0);
  EXPECT_TRUE(AI->SuggestMove(*Board.get(), m, 1));
  EXPECT_TRUE(m == Move(4, 0) || m == Move(-1, 0));
}

TEST_F(TicTacMasterTests, SuggestFirstMove00)
{
  Move m(-100,-100);
  EXPECT_TRUE(AI->SuggestMove(*Board.get(), m, 1));
  EXPECT_EQ(m, Move(0, 0));
}

TEST_F(TicTacMasterTests, PossibleMovesAreAllValid)
{
  EXPECT_EQ(Board->MakeMove(Move(1, 0)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(2, -1)), ResultMove_Ok);
  EXPECT_EQ(Board->MakeMove(Move(0, -1)), ResultMove_Ok);
  MovesSet_t moves = Board->GetAllPossibleMoves();
  for (MovesSet_t::const_iterator iter = moves.begin(); iter != moves.end(); ++iter)
  {
    EXPECT_TRUE(Board->CanMove(*iter));
  }
}


TEST_F(TicTacMasterTests, SomeScenario)
{
  std::string board =
    "o\n"
    "x     oxo\n"
    " x  ox ox\n"
    " ooxxxo x\n"
    "  x oox";
  
  Board = CreateTIcTacMemoryBoardSimple(board);
  CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get(), Move(0,0));
}


TEST_F(TicTacMasterTests, SomeScenario2)
{
  std::string board =
 "x\n"
 "  x o\n"
 "  xooo\n"
 " xooxxo\n"
 "  xxxox\n"
 " oxooxx\n"
 "   o o\n";
  
  Board = CreateTIcTacMemoryBoardSimple(board);
  CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get(), Move(0,0));
}

TEST_F(TicTacMasterTests, FreePlay3)
{
  Move m(-100,-100);
  while (1)
  {
    EXPECT_TRUE(AI->SuggestMove(*Board.get(), m, 3));
    ResultMove rm = Board->MakeMove(m);
    EXPECT_NE(rm, ResultMove_Invalid);
    CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get(), m);
    if (rm == ResultMove_Win)
      break;
  }
}

/*
TEST_F(TicTacMasterTests, FreePlay2)
{
  Move m(-100,-100);
  while (!Board->Finished())
  {
    EXPECT_TRUE(AI->SuggestMove(*Board.get(), m, 2));
    ResultMove rm = Board->MakeMove(m);
    EXPECT_NE(rm, ResultMove_Invalid);
    CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get(), m);

    if (rm == ResultMove_Win)
    {
            break;
    }
  }
}
*/


TEST_F(TicTacMasterTests, FreePlay4)
{
  Move m(-100,-100);
  while (1)
  {
    EXPECT_TRUE(AI->SuggestMove(*Board.get(), m, 4));
    ResultMove rm = Board->MakeMove(m);
    EXPECT_NE(rm, ResultMove_Invalid);
      CreateDummyTicTacVisualizer()->DisplayBoard(*Board.get(), m);
    if (rm == ResultMove_Win)
    {
      break;
    }
  }
}

int main(int argc, char *argv[])
{ 
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
