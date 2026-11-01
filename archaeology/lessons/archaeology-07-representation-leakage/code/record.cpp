#include <array>
#include <cstdio>

struct Knight
{
    int row;
    int column;
};

class Board
{
public:
    void mark(const Knight &knight)
    {
        visited[(knight.row - 1) * 8 + (knight.column - 1)] = true;
    }

    int visited_count() const
    {
        int count = 0;
        for (bool cell : visited)
        {
            if (cell)
            {
                count += 1;
            }
        }
        return count;
    }

private:
    std::array<bool, 64> visited = {};
};

struct Summary
{
    int playerRow;
    int playerColumn;
    int machineRow;
    int machineColumn;
    int visitedCount;
};

Summary run_scenario()
{
    Knight player{1, 1};
    Knight machine{8, 8};
    Board board;

    board.mark(player);
    player = Knight{2, 3};
    board.mark(player);
    machine = Knight{3, 4};
    board.mark(machine);

    return Summary{player.row, player.column, machine.row, machine.column,
                   board.visited_count()};
}

int main()
{
    Summary summary = run_scenario();
    std::printf("player=%d,%d machine=%d,%d visited=%d\n", summary.playerRow,
                summary.playerColumn, summary.machineRow, summary.machineColumn,
                summary.visitedCount);
    return 0;
}
