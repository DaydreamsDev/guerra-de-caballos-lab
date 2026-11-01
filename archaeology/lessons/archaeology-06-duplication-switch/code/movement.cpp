#include <array>
#include <cstdio>

struct Position
{
    int row;
    int column;

    bool operator==(const Position &other) const = default;
};

struct Delta
{
    int rowChange;
    int columnChange;
};

constexpr std::array<Delta, 8> moveDeltas = {
    Delta{-1, 2},
    Delta{-2, 1},
    Delta{-2, -1},
    Delta{-1, -2},
    Delta{1, -2},
    Delta{2, -1},
    Delta{2, 1},
    Delta{1, 2},
};

Position apply_switch(int moveNumber, Position position)
{
    switch (moveNumber)
    {
    case 1:
        return Position{position.row - 1, position.column + 2};
    case 2:
        return Position{position.row - 2, position.column + 1};
    case 3:
        return Position{position.row - 2, position.column - 1};
    case 4:
        return Position{position.row - 1, position.column - 2};
    case 5:
        return Position{position.row + 1, position.column - 2};
    case 6:
        return Position{position.row + 2, position.column - 1};
    case 7:
        return Position{position.row + 2, position.column + 1};
    case 8:
        return Position{position.row + 1, position.column + 2};
    default:
        return position;
    }
}

Position apply_table(int moveNumber, Position position)
{
    const Delta &delta = moveDeltas[moveNumber - 1];
    return Position{position.row + delta.rowChange, position.column + delta.columnChange};
}

int main()
{
    int comparisonCount = 0;
    int mismatchCount = 0;
    for (int moveNumber = 1; moveNumber <= 8; moveNumber += 1)
    {
        for (int row = -2; row <= 2; row += 1)
        {
            for (int column = -2; column <= 2; column += 1)
            {
                Position position{row, column};
                Position switchResult = apply_switch(moveNumber, position);
                Position tableResult = apply_table(moveNumber, position);
                comparisonCount += 1;
                if (!(switchResult == tableResult))
                {
                    mismatchCount += 1;
                }
            }
        }
    }
    std::printf("comparisons=%d mismatches=%d\n", comparisonCount, mismatchCount);
    return mismatchCount == 0 ? 0 : 1;
}
