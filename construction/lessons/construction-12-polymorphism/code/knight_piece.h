#pragma once

#include "piece.h"

#include <string>

class KnightPiece : public Piece
{
public:
    explicit KnightPiece(const std::string &roleName);
    int row() const override;
    int column() const override;
    void set_position(int row, int column) override;
    const char *role_name() const override;

private:
    int rowValue;
    int columnValue;
    std::string roleName;
};
