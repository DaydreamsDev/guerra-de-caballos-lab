#pragma once

class Board
{
public:
    Board();
    int row_count() const;
    int column_count() const;
    bool is_valid() const;
    void reset();
    bool was_visited(int row, int column) const;
    void mark_visited(int row, int column);

private:
    static constexpr int maximumRowCount = 16;
    static constexpr int maximumColumnCount = 16;
    static constexpr int activeRowCount = 8;
    static constexpr int activeColumnCount = 8;

    bool visited[maximumRowCount][maximumColumnCount];
};
