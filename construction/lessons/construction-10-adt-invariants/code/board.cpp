#include "board.h"

namespace
{
constexpr int maximumBoardRowCount = 16;
constexpr int maximumBoardColumnCount = 16;
constexpr int activeBoardRowCount = 8;
constexpr int activeBoardColumnCount = 8;
}

struct Board
{
    bool visited[maximumBoardRowCount][maximumBoardColumnCount];
};

Board *board_create()
{
    return new Board{};
}

void board_destroy(Board *board)
{
    delete board;
}

int board_row_count(const Board *)
{
    return activeBoardRowCount;
}

int board_column_count(const Board *)
{
    return activeBoardColumnCount;
}

bool board_is_valid(const Board *board)
{
    if (board == nullptr)
    {
        return false;
    }
    for (int row = 0; row < maximumBoardRowCount; row += 1)
    {
        for (int column = 0; column < maximumBoardColumnCount; column += 1)
        {
            bool outsideActiveArea =
                row < 1 || row > activeBoardRowCount || column < 1 ||
                column > activeBoardColumnCount;
            if (outsideActiveArea && board->visited[row][column])
            {
                return false;
            }
        }
    }
    return true;
}

void board_reset(Board *board)
{
    for (int row = 1; row <= activeBoardRowCount; row += 1)
    {
        for (int column = 1; column <= activeBoardColumnCount; column += 1)
        {
            board->visited[row][column] = false;
        }
    }
}

bool board_was_visited(const Board *board, int row, int column)
{
    if (row < 1 || row > activeBoardRowCount || column < 1 ||
        column > activeBoardColumnCount)
    {
        return false;
    }
    return board->visited[row][column];
}

void board_mark_visited(Board *board, int row, int column)
{
    if (row < 1 || row > activeBoardRowCount || column < 1 ||
        column > activeBoardColumnCount)
    {
        return;
    }
    board->visited[row][column] = true;
}
