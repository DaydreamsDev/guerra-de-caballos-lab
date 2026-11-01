#include "board.h"

struct Board
{
    bool visited[64];
};

Board *board_create()
{
    return new Board{};
}

void board_destroy(Board *board)
{
    delete board;
}

void board_reset(Board *board)
{
    for (int index = 0; index < 64; index += 1)
    {
        board->visited[index] = false;
    }
}

namespace
{
bool board_is_inside(int row, int column)
{
    return row >= 1 && row <= 8 && column >= 1 && column <= 8;
}

int board_cell_index(int row, int column)
{
    return (row - 1) * 8 + (column - 1);
}
}

void board_mark(Board *board, int row, int column)
{
    if (!board_is_inside(row, column))
    {
        return;
    }
    board->visited[board_cell_index(row, column)] = true;
}

bool board_is_visited(const Board *board, int row, int column)
{
    if (!board_is_inside(row, column))
    {
        return false;
    }
    return board->visited[board_cell_index(row, column)];
}

std::size_t board_visited_count(const Board *board)
{
    std::size_t count = 0;
    for (bool cell : board->visited)
    {
        if (cell)
        {
            count += 1;
        }
    }
    return count;
}
