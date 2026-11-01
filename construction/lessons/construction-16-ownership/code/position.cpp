#include "position.h"

int Position::row() const
{
    return rowValue;
}

int Position::column() const
{
    return columnValue;
}

void Position::set(int row, int column)
{
    rowValue = row;
    columnValue = column;
}
