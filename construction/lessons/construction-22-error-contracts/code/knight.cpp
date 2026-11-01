#include "knight.h"

Knight::Knight(const std::string &roleName)
    : position(), roleName(roleName)
{
}

int Knight::row() const
{
    return position.row();
}

int Knight::column() const
{
    return position.column();
}

void Knight::set_position(int row, int column)
{
    position.set(row, column);
}

const char *Knight::role_name() const
{
    return roleName.c_str();
}
