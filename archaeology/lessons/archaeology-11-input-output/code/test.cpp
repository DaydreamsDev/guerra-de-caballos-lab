#include "core.h"

#include <cstdio>

int main()
{
    GameResult result = run_game(11, "Learner");
    bool linesMatch = result.lines.size() == 8;
    bool winnerMatches = result.winner == "player";
    bool movesMatch = result.moveCount == 3;
    std::printf("lines=%zu winner=%s moves=%d\n", result.lines.size(),
                result.winner.c_str(), result.moveCount);
    if (!linesMatch || !winnerMatches || !movesMatch)
    {
        std::printf("test=failed\n");
        return 1;
    }
    std::printf("test=ok\n");
    return 0;
}
