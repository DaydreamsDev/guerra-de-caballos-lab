#include "game.h"

#include "console_transcript.h"
#include "game_engine.h"

int run_game(std::int64_t seed, const std::string &playerName)
{
    ConsoleTranscript transcript;
    GameEngine<8, 8> engine(transcript);
    return engine.run(seed, playerName);
}
