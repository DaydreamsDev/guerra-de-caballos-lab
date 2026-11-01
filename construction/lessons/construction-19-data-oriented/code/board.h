#pragma once

#include <array>
#include <cstdint>

constexpr int maximumRowCount = 16;
constexpr int maximumColumnCount = 16;
constexpr int maximumCellCount = maximumRowCount * maximumColumnCount;
constexpr int visitedWordCount = maximumCellCount / 64;

constexpr std::uint8_t playerOccupant = 1;
constexpr std::uint8_t machineOccupant = 2;

template <int RowCount, int ColumnCount>
class Board
{
public:
    Board()
        : visitedWords{},
          occupant{}
    {
    }

    int row_count() const
    {
        return RowCount;
    }

    int column_count() const
    {
        return ColumnCount;
    }

    bool is_valid() const
    {
        for (int row = 0; row < maximumRowCount; row += 1)
        {
            for (int column = 0; column < maximumColumnCount; column += 1)
            {
                bool outsideActiveArea =
                    row < 1 || row > RowCount || column < 1 || column > ColumnCount;
                int index = row * maximumColumnCount + column;
                bool visited = (visitedWords[index / 64] >> (index % 64)) & std::uint64_t{1};
                if (outsideActiveArea && (visited || occupant[index] != 0))
                {
                    return false;
                }
            }
        }
        return true;
    }

    void reset()
    {
        visitedWords.fill(0);
        occupant.fill(0);
    }

    bool was_visited(int row, int column) const
    {
        if (!is_inside(row, column))
        {
            return false;
        }
        int index = cell_index(row, column);
        return (visitedWords[index / 64] >> (index % 64)) & std::uint64_t{1};
    }

    void mark_visited(int row, int column)
    {
        if (!is_inside(row, column))
        {
            return;
        }
        int index = cell_index(row, column);
        visitedWords[index / 64] |= std::uint64_t{1} << (index % 64);
    }

    std::uint8_t occupant_at(int row, int column) const
    {
        if (!is_inside(row, column))
        {
            return 0;
        }
        return occupant[cell_index(row, column)];
    }

    void set_occupant(int row, int column, std::uint8_t occupantValue)
    {
        if (!is_inside(row, column))
        {
            return;
        }
        occupant[cell_index(row, column)] = occupantValue;
    }

    void clear_occupant(int row, int column)
    {
        set_occupant(row, column, 0);
    }

private:
    static_assert(RowCount >= 1 && RowCount <= maximumRowCount);
    static_assert(ColumnCount >= 1 && ColumnCount <= maximumColumnCount);

    static constexpr int cell_index(int row, int column)
    {
        return (row - 1) * maximumColumnCount + (column - 1);
    }

    static constexpr bool is_inside(int row, int column)
    {
        return row >= 1 && row <= RowCount && column >= 1 && column <= ColumnCount;
    }

    std::array<std::uint64_t, visitedWordCount> visitedWords;
    std::array<std::uint8_t, maximumCellCount> occupant;
};
