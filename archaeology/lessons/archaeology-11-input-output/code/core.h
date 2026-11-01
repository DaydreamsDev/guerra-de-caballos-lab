#pragma once

#include <string>
#include <vector>

struct GameResult
{
    std::vector<std::string> lines;
    std::string winner;
    int moveCount;
};

GameResult run_game(int seed, const std::string &playerName);
