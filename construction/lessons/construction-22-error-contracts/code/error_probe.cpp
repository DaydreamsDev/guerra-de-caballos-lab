#include "game_engine.h"
#include "recording_transcript.h"

#include <cstdio>

int main()
{
    RecordingTranscript transcript;
    GameEngine<8, 8> engine(transcript);
    Knight otherKnight("other");
    otherKnight.set_position(1, 1);

    MoveResult outsideResult =
        engine.evaluate_move(otherKnight, MoveCandidate{1, 0, 0});
    MoveResult captureResult =
        engine.evaluate_move(otherKnight, MoveCandidate{1, 1, 1});
    MoveResult legalResult =
        engine.evaluate_move(otherKnight, MoveCandidate{1, 2, 3});

    std::printf("outside=%d capture=%d legal=%d\n",
                static_cast<int>(outsideResult.outcome),
                static_cast<int>(captureResult.outcome),
                static_cast<int>(legalResult.outcome));
    return 0;
}
