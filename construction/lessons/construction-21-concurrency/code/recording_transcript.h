#pragma once

#include "transcript_port.h"

#include <string>

class RecordingTranscript : public TranscriptPort
{
public:
    int gameHeaderCount = 0;
    int turnHeaderCount = 0;
    int illegalAttemptCount = 0;
    int captureAttemptCount = 0;
    int legalAttemptCount = 0;
    int moveCount = 0;
    int noLegalMoveCount = 0;
    int winnerCount = 0;
    int finalMoveCount = 0;
    std::string winnerName;

    void game_header(std::int64_t seed, const std::string &playerName, int playerRoll,
                     int machineRoll, bool playerIsActive, int playerRow, int playerColumn,
                     int machineRow, int machineColumn) override
    {
        (void)seed;
        (void)playerName;
        (void)playerRoll;
        (void)machineRoll;
        (void)playerIsActive;
        (void)playerRow;
        (void)playerColumn;
        (void)machineRow;
        (void)machineColumn;
        gameHeaderCount += 1;
    }

    void turn_header(int turnNumber, const char *roleName, int row, int column,
                     const int moveOrder[moveCandidateCount]) override
    {
        (void)turnNumber;
        (void)roleName;
        (void)row;
        (void)column;
        (void)moveOrder;
        turnHeaderCount += 1;
    }

    void illegal_attempt(int moveNumber, int destinationRow, int destinationColumn,
                         const char *reason) override
    {
        (void)moveNumber;
        (void)destinationRow;
        (void)destinationColumn;
        (void)reason;
        illegalAttemptCount += 1;
    }

    void capture_attempt(int moveNumber, int destinationRow, int destinationColumn,
                         const char *otherKnightName) override
    {
        (void)moveNumber;
        (void)destinationRow;
        (void)destinationColumn;
        (void)otherKnightName;
        captureAttemptCount += 1;
    }

    void legal_attempt(int moveNumber, int destinationRow, int destinationColumn) override
    {
        (void)moveNumber;
        (void)destinationRow;
        (void)destinationColumn;
        legalAttemptCount += 1;
    }

    void move(int moveNumber, const char *roleName, int fromRow, int fromColumn, int toRow,
              int toColumn, bool wasCapture) override
    {
        (void)moveNumber;
        (void)roleName;
        (void)fromRow;
        (void)fromColumn;
        (void)toRow;
        (void)toColumn;
        (void)wasCapture;
        moveCount += 1;
    }

    void no_legal_move(const char *roleName) override
    {
        (void)roleName;
        noLegalMoveCount += 1;
    }

    void winner(const char *winnerNameValue, int moveCountValue) override
    {
        winnerCount += 1;
        winnerName = winnerNameValue;
        finalMoveCount = moveCountValue;
    }
};
