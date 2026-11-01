#pragma once

class Position
{
public:
    int row() const;
    int column() const;
    void set(int row, int column);

private:
    int rowValue = 0;
    int columnValue = 0;
};
