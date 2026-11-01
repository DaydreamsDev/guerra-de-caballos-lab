#include "console_transcript.h"

#include <cstdio>

void ConsoleTranscript::game_header(std::int64_t seed, const std::string &playerName,
                                    int playerRoll, int machineRoll, bool playerIsActive,
                                    int playerRow, int playerColumn, int machineRow,
                                    int machineColumn)
{
    std::printf("seed=%lld\n", static_cast<long long>(seed));
    std::printf("player=%s\n", playerName.c_str());
    std::printf("roll_player=%d\n", playerRoll);
    std::printf("roll_machine=%d\n", machineRoll);
    std::printf("start=%s\n", playerIsActive ? "player" : "machine");
    std::printf("initial_player=%d,%d\n", playerRow, playerColumn);
    std::printf("initial_machine=%d,%d\n", machineRow, machineColumn);
}

void ConsoleTranscript::turn_header(int turnNumber, const char *roleName, int row, int column,
                                    const int moveOrder[moveCandidateCount])
{
    std::printf("turn=%d who=%s at=%d,%d order=", turnNumber, roleName, row, column);
    for (int orderIndex = 0; orderIndex < moveCandidateCount; orderIndex += 1)
    {
        std::printf("%d", moveOrder[orderIndex]);
        if (orderIndex + 1 < moveCandidateCount)
        {
            std::printf(",");
        }
    }
    std::printf("\n");
}

void ConsoleTranscript::illegal_attempt(int moveNumber, int destinationRow,
                                        int destinationColumn, const char *reason)
{
    std::printf("  try=%d to=%d,%d illegal=%s\n", moveNumber, destinationRow,
                destinationColumn, reason);
}

void ConsoleTranscript::capture_attempt(int moveNumber, int destinationRow,
                                        int destinationColumn,
                                        const char *otherKnightName)
{
    std::printf("  try=%d to=%d,%d capture=%s\n", moveNumber, destinationRow,
                destinationColumn, otherKnightName);
}

void ConsoleTranscript::legal_attempt(int moveNumber, int destinationRow,
                                      int destinationColumn)
{
    std::printf("  try=%d to=%d,%d legal\n", moveNumber, destinationRow, destinationColumn);
}

void ConsoleTranscript::move(int moveNumber, const char *roleName, int fromRow, int fromColumn,
                             int toRow, int toColumn, bool wasCapture)
{
    std::printf("move=%d %s %d,%d->%d,%d", moveNumber, roleName, fromRow, fromColumn, toRow,
                toColumn);
    if (wasCapture)
    {
        std::printf(" capture");
    }
    std::printf("\n");
}

void ConsoleTranscript::no_legal_move(const char *roleName)
{
    std::printf("  no_legal_move trapped=%s\n", roleName);
}

void ConsoleTranscript::winner(const char *winnerName, int moveCount)
{
    std::printf("winner=%s\n", winnerName);
    std::printf("moves=%d\n", moveCount);
}
