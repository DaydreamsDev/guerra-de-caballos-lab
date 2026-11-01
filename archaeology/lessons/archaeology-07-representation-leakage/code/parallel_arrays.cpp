#include <array>
#include <cstdio>

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
    int playerRow = 1;
    int playerColumn = 1;
    int machineRow = 8;
    int machineColumn = 8;
    std::array<bool, 64> visited = {};

    visited[(playerRow - 1) * 8 + (playerColumn - 1)] = true;
    playerRow = 2;
    playerColumn = 3;
    visited[(playerRow - 1) * 8 + (playerColumn - 1)] = true;
    machineRow = 3;
    machineColumn = 4;
    visited[(machineRow - 1) * 8 + (machineColumn - 1)] = true;

    int visitedCount = 0;
    for (bool cell : visited)
    {
        if (cell)
        {
            visitedCount += 1;
        }
    }
    return Summary{playerRow, playerColumn, machineRow, machineColumn, visitedCount};
}

int main()
{
    Summary summary = run_scenario();
    std::printf("player=%d,%d machine=%d,%d visited=%d\n", summary.playerRow,
                summary.playerColumn, summary.machineRow, summary.machineColumn,
                summary.visitedCount);
    return 0;
}
