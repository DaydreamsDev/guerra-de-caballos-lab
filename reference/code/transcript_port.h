#pragma once

#include "move_generator.h"

#include <cstdint>
#include <string>

class TranscriptPort
{
public:
    virtual ~TranscriptPort() = default;

    virtual void game_header(std::int64_t seed, const std::string &playerName,
                             int playerRoll, int machineRoll, bool playerIsActive,
                             int playerRow, int playerColumn, int machineRow,
                             int machineColumn) = 0;

    virtual void turn_header(int turnNumber, const char *roleName, int row, int column,
                             const int moveOrder[moveCandidateCount]) = 0;

    virtual void illegal_attempt(int moveNumber, int destinationRow, int destinationColumn,
                                 const char *reason) = 0;

    virtual void capture_attempt(int moveNumber, int destinationRow, int destinationColumn,
                                 const char *otherKnightName) = 0;

    virtual void legal_attempt(int moveNumber, int destinationRow, int destinationColumn) = 0;

    virtual void move(int moveNumber, const char *roleName, int fromRow, int fromColumn,
                      int toRow, int toColumn, bool wasCapture) = 0;

    virtual void no_legal_move(const char *roleName) = 0;

    virtual void winner(const char *winnerName, int moveCount) = 0;
};
