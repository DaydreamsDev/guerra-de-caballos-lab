#include "knight_piece.h"

KnightPiece::KnightPiece(const std::string &roleName)
    : rowValue(0), columnValue(0), roleName(roleName)
{
}

int KnightPiece::row() const
{
    return rowValue;
}

int KnightPiece::column() const
{
    return columnValue;
}

void KnightPiece::set_position(int row, int column)
{
    rowValue = row;
    columnValue = column;
}

const char *KnightPiece::role_name() const
{
    return roleName.c_str();
}
