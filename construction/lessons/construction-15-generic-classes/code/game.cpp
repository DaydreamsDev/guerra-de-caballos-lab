#include "game.h"

#include "game_engine.h"
#include "knight.h"

int run_game(std::int64_t seed, const std::string &playerName)
{
    GameEngine<8, 8> engine;
    return engine.run(seed, playerName);
}
