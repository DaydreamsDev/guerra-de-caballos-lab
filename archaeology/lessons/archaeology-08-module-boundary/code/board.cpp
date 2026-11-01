#include "board.h"

Board::Board()
    : visited{}
{
}

void Board::reset()
{
    for (int index = 0; index < cellCount; index += 1)
    {
        visited[index] = false;
    }
}

bool Board::is_inside(int row, int column) const
{
    return row >= 1 && row <= rowCount && column >= 1 && column <= columnCount;
}

int Board::cell_index(int row, int column) const
{
    return (row - 1) * columnCount + (column - 1);
}

void Board::mark(int row, int column)
{
    if (!is_inside(row, column))
    {
        return;
    }
    visited[cell_index(row, column)] = true;
}

bool Board::is_visited(int row, int column) const
{
    if (!is_inside(row, column))
    {
        return false;
    }
    return visited[cell_index(row, column)];
}

std::size_t Board::visited_count() const
{
    std::size_t count = 0;
    for (bool cell : visited)
    {
        if (cell)
        {
            count += 1;
        }
    }
    return count;
}
