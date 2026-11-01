#include "game.h"

#include "board.h"
#include "random_sequence.h"

#include <cstdio>
#include <string>

namespace
{
constexpr int moveOrderMinimum = 1;
constexpr int moveOrderMaximum = 8;
constexpr int rollMinimum = 1;
constexpr int rollMaximum = 50;
constexpr int maximumMoveCount = 200;

constexpr int knightRowChangePerMove[moveOrderMaximum] = {-1, -2, -2, -1, 1, 2, 2, 1};
constexpr int knightColumnChangePerMove[moveOrderMaximum] = {2, 1, -1, -2, -2, -1, 1, 2};

struct Knight
{
    int row;
    int column;
};

enum class MoveOutcome
{
    outsideBoard,
    visited,
    capture,
    legal
};

int convertCellNumberToRow(int cellNumber)
{
    return cellNumber / board_column_count() + 1;
}

int convertCellNumberToColumn(int cellNumber)
{
    return cellNumber % board_column_count() + 1;
}

void initializePositions(Knight *playerKnight, Knight *machineKnight)
{
    int boardCellCount = board_row_count() * board_column_count();
    int playerCellNumber = static_cast<int>(random_below(boardCellCount));
    int machineCellNumber = static_cast<int>(random_below(boardCellCount));
    while (machineCellNumber == playerCellNumber)
    {
        machineCellNumber = static_cast<int>(random_below(boardCellCount));
    }
    playerKnight->row = convertCellNumberToRow(playerCellNumber);
    playerKnight->column = convertCellNumberToColumn(playerCellNumber);
    machineKnight->row = convertCellNumberToRow(machineCellNumber);
    machineKnight->column = convertCellNumberToColumn(machineCellNumber);
}

void initializeRolls(int &playerRoll, int &machineRoll)
{
    playerRoll = random_integer(rollMinimum, rollMaximum);
    machineRoll = random_integer(rollMinimum, rollMaximum);
    while (machineRoll == playerRoll)
    {
        machineRoll = random_integer(rollMinimum, rollMaximum);
    }
}

void printGameHeader(std::int64_t seedValue, const std::string &playerName, int playerRoll,
                     int machineRoll, bool playerIsActive, const Knight *playerKnight,
                     const Knight *machineKnight)
{
    std::printf("seed=%lld\n", static_cast<long long>(seedValue));
    std::printf("player=%s\n", playerName.c_str());
    std::printf("roll_player=%d\n", playerRoll);
    std::printf("roll_machine=%d\n", machineRoll);
    std::printf("start=%s\n", playerIsActive ? "player" : "machine");
    std::printf("initial_player=%d,%d\n", playerKnight->row, playerKnight->column);
    std::printf("initial_machine=%d,%d\n", machineKnight->row, machineKnight->column);
}

void buildMoveOrder(int startingMoveNumber, int moveOrder[moveOrderMaximum])
{
    for (int orderIndex = 0; orderIndex < moveOrderMaximum; orderIndex += 1)
    {
        moveOrder[orderIndex] =
            (startingMoveNumber - 1 + orderIndex) % moveOrderMaximum + 1;
    }
}

void printTurnHeader(int turnNumber, const char *activeKnightName, const Knight *activeKnight,
                     const int moveOrder[moveOrderMaximum])
{
    std::printf("turn=%d who=%s at=%d,%d order=", turnNumber, activeKnightName,
                activeKnight->row, activeKnight->column);
    for (int orderIndex = 0; orderIndex < moveOrderMaximum; orderIndex += 1)
    {
        std::printf("%d", moveOrder[orderIndex]);
        if (orderIndex + 1 < moveOrderMaximum)
        {
            std::printf(",");
        }
    }
    std::printf("\n");
}

void printIllegalAttempt(int moveNumber, int destinationRow, int destinationColumn,
                         const char *reason)
{
    std::printf("  try=%d to=%d,%d illegal=%s\n", moveNumber, destinationRow,
                destinationColumn, reason);
}

void printCaptureAttempt(int moveNumber, int destinationRow, int destinationColumn,
                         const char *otherKnightName)
{
    std::printf("  try=%d to=%d,%d capture=%s\n", moveNumber, destinationRow,
                destinationColumn, otherKnightName);
}

void printLegalAttempt(int moveNumber, int destinationRow, int destinationColumn)
{
    std::printf("  try=%d to=%d,%d legal\n", moveNumber, destinationRow, destinationColumn);
}

void printMove(int moveNumber, const char *knightName, const Knight *fromKnight,
               int toRow, int toColumn, bool wasCapture)
{
    std::printf("move=%d %s %d,%d->%d,%d", moveNumber, knightName, fromKnight->row,
                fromKnight->column, toRow, toColumn);
    if (wasCapture)
    {
        std::printf(" capture");
    }
    std::printf("\n");
}

Knight *selectActiveKnight(bool playerIsActive, Knight *playerKnight, Knight *machineKnight)
{
    return playerIsActive ? playerKnight : machineKnight;
}

const Knight *selectOtherKnight(bool playerIsActive, const Knight *playerKnight,
                                const Knight *machineKnight)
{
    return playerIsActive ? machineKnight : playerKnight;
}

void moveKnight(Knight *knight, int destinationRow, int destinationColumn)
{
    knight->row = destinationRow;
    knight->column = destinationColumn;
}

MoveOutcome tryMove(const Knight *activeKnight, const Knight *otherKnight, int moveNumber,
                    int &destinationRow, int &destinationColumn)
{
    destinationRow = activeKnight->row + knightRowChangePerMove[moveNumber - 1];
    destinationColumn = activeKnight->column + knightColumnChangePerMove[moveNumber - 1];

    if (destinationRow < 1 || destinationRow > board_row_count() ||
        destinationColumn < 1 || destinationColumn > board_column_count())
    {
        return MoveOutcome::outsideBoard;
    }
    if (destinationRow == otherKnight->row && destinationColumn == otherKnight->column)
    {
        return MoveOutcome::capture;
    }
    if (board_was_visited(destinationRow, destinationColumn))
    {
        return MoveOutcome::visited;
    }
    return MoveOutcome::legal;
}

void playTurn(bool &playerIsActive, Knight *playerKnight, Knight *machineKnight,
              int &successfulMoveCount, std::string &winnerName)
{
    int startingMoveNumber = random_integer(moveOrderMinimum, moveOrderMaximum);
    int moveOrder[moveOrderMaximum] = {};
    buildMoveOrder(startingMoveNumber, moveOrder);

    const char *activeKnightName = playerIsActive ? "player" : "machine";
    const char *otherKnightName = playerIsActive ? "machine" : "player";
    Knight *activeKnight = selectActiveKnight(playerIsActive, playerKnight, machineKnight);
    const Knight *otherKnight =
        selectOtherKnight(playerIsActive, playerKnight, machineKnight);

    printTurnHeader(successfulMoveCount + 1, activeKnightName, activeKnight, moveOrder);

    bool moveWasAccepted = false;
    for (int attemptIndex = 0;
         attemptIndex < moveOrderMaximum && !moveWasAccepted;
         attemptIndex += 1)
    {
        int attemptedMoveNumber = moveOrder[attemptIndex];
        int destinationRow = 0;
        int destinationColumn = 0;
        MoveOutcome outcome = tryMove(activeKnight, otherKnight, attemptedMoveNumber,
                                      destinationRow, destinationColumn);

        if (outcome == MoveOutcome::outsideBoard)
        {
            printIllegalAttempt(attemptedMoveNumber, destinationRow, destinationColumn,
                                "outside");
            continue;
        }
        if (outcome == MoveOutcome::visited)
        {
            printIllegalAttempt(attemptedMoveNumber, destinationRow, destinationColumn,
                                "visited");
            continue;
        }
        if (outcome == MoveOutcome::capture)
        {
            printCaptureAttempt(attemptedMoveNumber, destinationRow, destinationColumn,
                                otherKnightName);
            successfulMoveCount += 1;
            printMove(successfulMoveCount, activeKnightName, activeKnight, destinationRow,
                      destinationColumn, true);
            board_mark_visited(destinationRow, destinationColumn);
            winnerName = activeKnightName;
            moveWasAccepted = true;
            continue;
        }

        printLegalAttempt(attemptedMoveNumber, destinationRow, destinationColumn);
        successfulMoveCount += 1;
        printMove(successfulMoveCount, activeKnightName, activeKnight, destinationRow,
                  destinationColumn, false);
        board_mark_visited(destinationRow, destinationColumn);
        moveKnight(activeKnight, destinationRow, destinationColumn);
        playerIsActive = !playerIsActive;
        moveWasAccepted = true;
    }

    if (!moveWasAccepted)
    {
        std::printf("  no_legal_move trapped=%s\n", activeKnightName);
        winnerName = otherKnightName;
    }
}

void playGame(Knight *playerKnight, Knight *machineKnight, bool playerIsActive,
              int &successfulMoveCount, std::string &winnerName)
{
    while (winnerName.empty() && successfulMoveCount < maximumMoveCount)
    {
        playTurn(playerIsActive, playerKnight, machineKnight, successfulMoveCount,
                 winnerName);
    }

    if (winnerName.empty())
    {
        winnerName = "draw";
    }
    std::printf("winner=%s\n", winnerName.c_str());
    std::printf("moves=%d\n", successfulMoveCount);
}
}

int run_game(std::int64_t seed, const std::string &playerName)
{
    random_set_seed(seed);
    board_reset();

    Knight playerKnight = {};
    Knight machineKnight = {};
    initializePositions(&playerKnight, &machineKnight);

    int playerRoll = 0;
    int machineRoll = 0;
    initializeRolls(playerRoll, machineRoll);

    bool playerIsActive = playerRoll > machineRoll;
    printGameHeader(seed, playerName, playerRoll, machineRoll, playerIsActive,
                    &playerKnight, &machineKnight);

    int successfulMoveCount = 0;
    std::string winnerName;
    playGame(&playerKnight, &machineKnight, playerIsActive, successfulMoveCount, winnerName);
    return 0;
}
