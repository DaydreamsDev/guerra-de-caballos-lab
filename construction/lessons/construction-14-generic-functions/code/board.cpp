#include "board.h"

Board::Board()
    : visited{}
{
}

int Board::row_count() const
{
    return activeRowCount;
}

int Board::column_count() const
{
    return activeColumnCount;
}

bool Board::is_valid() const
{
    for (int row = 0; row < maximumRowCount; row += 1)
    {
        for (int column = 0; column < maximumColumnCount; column += 1)
        {
            bool outsideActiveArea =
                row < 1 || row > activeRowCount || column < 1 ||
                column > activeColumnCount;
            if (outsideActiveArea && visited[row][column])
            {
                return false;
            }
        }
    }
    return true;
}

void Board::reset()
{
    for (int row = 1; row <= activeRowCount; row += 1)
    {
        for (int column = 1; column <= activeColumnCount; column += 1)
        {
            visited[row][column] = false;
        }
    }
}

bool Board::was_visited(int row, int column) const
{
    if (row < 1 || row > activeRowCount || column < 1 ||
        column > activeColumnCount)
    {
        return false;
    }
    return visited[row][column];
}

void Board::mark_visited(int row, int column)
{
    if (row < 1 || row > activeRowCount || column < 1 ||
        column > activeColumnCount)
    {
        return;
    }
    visited[row][column] = true;
}
