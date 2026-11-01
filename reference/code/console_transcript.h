#pragma once

#include "transcript_port.h"

class ConsoleTranscript : public TranscriptPort
{
public:
    void game_header(std::int64_t seed, const std::string &playerName, int playerRoll,
                     int machineRoll, bool playerIsActive, int playerRow, int playerColumn,
                     int machineRow, int machineColumn) override;

    void turn_header(int turnNumber, const char *roleName, int row, int column,
                     const int moveOrder[moveCandidateCount]) override;

    void illegal_attempt(int moveNumber, int destinationRow, int destinationColumn,
                         const char *reason) override;

    void capture_attempt(int moveNumber, int destinationRow, int destinationColumn,
                         const char *otherKnightName) override;

    void legal_attempt(int moveNumber, int destinationRow, int destinationColumn) override;

    void move(int moveNumber, const char *roleName, int fromRow, int fromColumn, int toRow,
              int toColumn, bool wasCapture) override;

    void no_legal_move(const char *roleName) override;

    void winner(const char *winnerName, int moveCount) override;
};
