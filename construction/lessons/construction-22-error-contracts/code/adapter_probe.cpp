#include "game_engine.h"
#include "recording_transcript.h"

#include <cstdio>

int main()
{
    RecordingTranscript transcript;
    GameEngine<8, 8> engine(transcript);
    engine.run(1, "Learner");
    std::printf("headers=%d turns=%d illegal=%d capture=%d legal=%d moves=%d no_legal=%d "
                "winners=%d winner=%s final_moves=%d\n",
                transcript.gameHeaderCount, transcript.turnHeaderCount,
                transcript.illegalAttemptCount, transcript.captureAttemptCount,
                transcript.legalAttemptCount, transcript.moveCount,
                transcript.noLegalMoveCount, transcript.winnerCount,
                transcript.winnerName.c_str(), transcript.finalMoveCount);
    return 0;
}
