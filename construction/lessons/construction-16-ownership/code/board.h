#pragma once

template <int RowCount, int ColumnCount>
class Board
{
public:
    Board()
        : visited{}
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
                if (outsideActiveArea && visited[row][column])
                {
                    return false;
                }
            }
        }
        return true;
    }

    void reset()
    {
        for (int row = 1; row <= RowCount; row += 1)
        {
            for (int column = 1; column <= ColumnCount; column += 1)
            {
                visited[row][column] = false;
            }
        }
    }

    bool was_visited(int row, int column) const
    {
        if (row < 1 || row > RowCount || column < 1 || column > ColumnCount)
        {
            return false;
        }
        return visited[row][column];
    }

    void mark_visited(int row, int column)
    {
        if (row < 1 || row > RowCount || column < 1 || column > ColumnCount)
        {
            return;
        }
        visited[row][column] = true;
    }

private:
    static constexpr int maximumRowCount = 16;
    static constexpr int maximumColumnCount = 16;

    static_assert(RowCount >= 1 && RowCount <= maximumRowCount);
    static_assert(ColumnCount >= 1 && ColumnCount <= maximumColumnCount);

    bool visited[maximumRowCount][maximumColumnCount];
};
