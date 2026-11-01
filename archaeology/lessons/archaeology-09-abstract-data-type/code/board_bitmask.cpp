#include "board.h"

#include <bit>
#include <cstdint>

struct Board
{
    std::uint64_t visited;
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
    board->visited = 0;
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
    int index = board_cell_index(row, column);
    board->visited |= std::uint64_t{1} << index;
}

bool board_is_visited(const Board *board, int row, int column)
{
    if (!board_is_inside(row, column))
    {
        return false;
    }
    int index = board_cell_index(row, column);
    return ((board->visited >> index) & std::uint64_t{1}) != 0;
}

std::size_t board_visited_count(const Board *board)
{
    return static_cast<std::size_t>(std::popcount(board->visited));
}
