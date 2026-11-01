#include "board.h"

namespace
{
constexpr int maximumBoardRowCount = 16;
constexpr int maximumBoardColumnCount = 16;
constexpr int activeBoardRowCount = 8;
constexpr int activeBoardColumnCount = 8;

struct BoardCell
{
    bool visited;
};

BoardCell boardCells[maximumBoardRowCount][maximumBoardColumnCount] = {};
}

int board_row_count()
{
    return activeBoardRowCount;
}

int board_column_count()
{
    return activeBoardColumnCount;
}

void board_reset()
{
    for (int row = 1; row <= activeBoardRowCount; row += 1)
    {
        for (int column = 1; column <= activeBoardColumnCount; column += 1)
        {
            boardCells[row][column].visited = false;
        }
    }
}

bool board_was_visited(int row, int column)
{
    if (row < 1 || row > activeBoardRowCount || column < 1 ||
        column > activeBoardColumnCount)
    {
        return false;
    }
    return boardCells[row][column].visited;
}

void board_mark_visited(int row, int column)
{
    if (row < 1 || row > activeBoardRowCount || column < 1 ||
        column > activeBoardColumnCount)
    {
        return;
    }
    boardCells[row][column].visited = true;
}
