#pragma once

#include <cstddef>

class Board
{
public:
    Board();
    void reset();
    void mark(int row, int column);
    bool is_visited(int row, int column) const;
    std::size_t visited_count() const;

private:
    static constexpr int rowCount = 8;
    static constexpr int columnCount = 8;
    static constexpr int cellCount = rowCount * columnCount;

    int cell_index(int row, int column) const;
    bool is_inside(int row, int column) const;

    bool visited[cellCount];
};
