#pragma once

#include "position.h"

#include <string>

class Knight
{
public:
    explicit Knight(const std::string &roleName);
    int row() const;
    int column() const;
    void set_position(int row, int column);
    const char *role_name() const;

private:
    Position position;
    std::string roleName;
};
