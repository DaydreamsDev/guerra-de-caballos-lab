#include "core.h"

#include <cstdint>
#include <string>

GameResult run_game(int seed, const std::string &playerName)
{
    std::uint64_t state = static_cast<std::uint64_t>(seed);
    int position = 0;
    std::vector<std::string> lines;
    lines.push_back("seed=" + std::to_string(seed));
    lines.push_back("player=" + playerName);
    lines.push_back("start=player");

    for (int moveNumber = 1; moveNumber <= 3; moveNumber += 1)
    {
        state = state * 6364136223846793005ULL + 1442695040888963407ULL;
        position = (position + static_cast<int>(state % 3) + 1) % 8;
        lines.push_back("move=" + std::to_string(moveNumber) + " position=" +
                        std::to_string(position));
    }

    lines.push_back("winner=player");
    lines.push_back("moves=3");
    return GameResult{lines, "player", 3};
}
