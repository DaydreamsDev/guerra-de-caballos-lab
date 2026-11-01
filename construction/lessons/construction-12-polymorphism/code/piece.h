#pragma once

class Piece
{
public:
    virtual ~Piece() = default;
    virtual int row() const = 0;
    virtual int column() const = 0;
    virtual void set_position(int row, int column) = 0;
    virtual const char *role_name() const = 0;
};
