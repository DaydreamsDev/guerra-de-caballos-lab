#pragma once

#include "board.h"
#include "knight.h"
#include "move_generator.h"
#include "random_sequence.h"

#include <array>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>

template <typename PieceType>
concept PositionedPiece = requires(PieceType piece, const PieceType constPiece) {
    { constPiece.row() } -> std::convertible_to<int>;
    { constPiece.column() } -> std::convertible_to<int>;
    { piece.set_position(1, 1) } -> std::same_as<void>;
    { constPiece.role_name() } -> std::convertible_to<const char *>;
};

static_assert(PositionedPiece<Knight>);

enum class MoveOutcome
{
    outsideBoard,
    visited,
    capture,
    legal
};

constexpr int maximumMoveCount = 200;

template <int RowCount, int ColumnCount>
class GameEngine
{
public:
    GameEngine()
        : board(std::make_unique<Board<RowCount, ColumnCount>>()),
          playerKnight(std::make_unique<Knight>("player")),
          machineKnight(std::make_unique<Knight>("machine"))
    {
    }

    GameEngine(const GameEngine &) = delete;
    GameEngine &operator=(const GameEngine &) = delete;
    GameEngine(GameEngine &&) = default;
    GameEngine &operator=(GameEngine &&) = default;

    int run(std::int64_t seed, const std::string &playerName)
    {
        random_set_seed(seed);
        board->reset();
        initializePositions();

        int playerRoll = 0;
        int machineRoll = 0;
        initializeRolls(playerRoll, machineRoll);

        bool playerIsActive = playerRoll > machineRoll;
        printGameHeader(seed, playerName, playerRoll, machineRoll, playerIsActive);

        int successfulMoveCount = 0;
        std::string winnerName;
        playGame(playerIsActive, successfulMoveCount, winnerName);
        return 0;
    }

private:
    std::unique_ptr<Board<RowCount, ColumnCount>> board;
    std::unique_ptr<Knight> playerKnight;
    std::unique_ptr<Knight> machineKnight;

    int convertCellNumberToRow(int cellNumber) const
    {
        return cellNumber / board->column_count() + 1;
    }

    int convertCellNumberToColumn(int cellNumber) const
    {
        return cellNumber % board->column_count() + 1;
    }

    void initializePositions()
    {
        int boardCellCount = board->row_count() * board->column_count();
        int playerCellNumber = static_cast<int>(random_below(boardCellCount));
        int machineCellNumber = static_cast<int>(random_below(boardCellCount));
        while (machineCellNumber == playerCellNumber)
        {
            machineCellNumber = static_cast<int>(random_below(boardCellCount));
        }
        playerKnight->set_position(convertCellNumberToRow(playerCellNumber),
                                  convertCellNumberToColumn(playerCellNumber));
        machineKnight->set_position(convertCellNumberToRow(machineCellNumber),
                                   convertCellNumberToColumn(machineCellNumber));
    }

    void initializeRolls(int &playerRoll, int &machineRoll)
    {
        playerRoll = random_integer(1, 50);
        machineRoll = random_integer(1, 50);
        while (machineRoll == playerRoll)
        {
            machineRoll = random_integer(1, 50);
        }
    }

    void printGameHeader(std::int64_t seedValue, const std::string &playerName,
                         int playerRoll, int machineRoll, bool playerIsActive) const
    {
        std::printf("seed=%lld\n", static_cast<long long>(seedValue));
        std::printf("player=%s\n", playerName.c_str());
        std::printf("roll_player=%d\n", playerRoll);
        std::printf("roll_machine=%d\n", machineRoll);
        std::printf("start=%s\n", playerIsActive ? "player" : "machine");
        std::printf("initial_player=%d,%d\n", playerKnight->row(), playerKnight->column());
        std::printf("initial_machine=%d,%d\n", machineKnight->row(), machineKnight->column());
    }

    void buildMoveOrder(int startingMoveNumber, int moveOrder[moveCandidateCount]) const
    {
        for (int orderIndex = 0; orderIndex < moveCandidateCount; orderIndex += 1)
        {
            moveOrder[orderIndex] =
                (startingMoveNumber - 1 + orderIndex) % moveCandidateCount + 1;
        }
    }

    void printTurnHeader(int turnNumber, const Knight *activeKnight,
                         const int moveOrder[moveCandidateCount]) const
    {
        std::printf("turn=%d who=%s at=%d,%d order=", turnNumber,
                    activeKnight->role_name(), activeKnight->row(),
                    activeKnight->column());
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

    void printIllegalAttempt(int moveNumber, int destinationRow, int destinationColumn,
                             const char *reason) const
    {
        std::printf("  try=%d to=%d,%d illegal=%s\n", moveNumber, destinationRow,
                    destinationColumn, reason);
    }

    void printCaptureAttempt(int moveNumber, int destinationRow, int destinationColumn,
                             const char *otherKnightName) const
    {
        std::printf("  try=%d to=%d,%d capture=%s\n", moveNumber, destinationRow,
                    destinationColumn, otherKnightName);
    }

    void printLegalAttempt(int moveNumber, int destinationRow, int destinationColumn) const
    {
        std::printf("  try=%d to=%d,%d legal\n", moveNumber, destinationRow,
                    destinationColumn);
    }

    void printMove(int moveNumber, const Knight *fromKnight, int toRow, int toColumn,
                   bool wasCapture) const
    {
        std::printf("move=%d %s %d,%d->%d,%d", moveNumber, fromKnight->role_name(),
                    fromKnight->row(), fromKnight->column(), toRow, toColumn);
        if (wasCapture)
        {
            std::printf(" capture");
        }
        std::printf("\n");
    }

    Knight *selectActiveKnight(bool playerIsActive)
    {
        return playerIsActive ? playerKnight.get() : machineKnight.get();
    }

    const Knight *selectOtherKnight(bool playerIsActive) const
    {
        return playerIsActive ? machineKnight.get() : playerKnight.get();
    }

    void moveKnight(Knight *knight, int destinationRow, int destinationColumn)
    {
        knight->set_position(destinationRow, destinationColumn);
    }

    MoveOutcome tryMove(const Knight *otherKnight, const MoveCandidate &candidate,
                        int &destinationRow, int &destinationColumn) const
    {
        destinationRow = candidate.destinationRow;
        destinationColumn = candidate.destinationColumn;

        if (destinationRow < 1 || destinationRow > board->row_count() ||
            destinationColumn < 1 || destinationColumn > board->column_count())
        {
            return MoveOutcome::outsideBoard;
        }
        if (destinationRow == otherKnight->row() &&
            destinationColumn == otherKnight->column())
        {
            return MoveOutcome::capture;
        }
        if (board->was_visited(destinationRow, destinationColumn))
        {
            return MoveOutcome::visited;
        }
        return MoveOutcome::legal;
    }

    void playTurn(bool &playerIsActive, int &successfulMoveCount, std::string &winnerName)
    {
        int startingMoveNumber = random_integer(1, moveCandidateCount);
        int moveOrder[moveCandidateCount] = {};
        buildMoveOrder(startingMoveNumber, moveOrder);

        Knight *activeKnight = selectActiveKnight(playerIsActive);
        const Knight *otherKnight = selectOtherKnight(playerIsActive);
        const char *activeKnightName = activeKnight->role_name();
        const char *otherKnightName = otherKnight->role_name();

        printTurnHeader(successfulMoveCount + 1, activeKnight, moveOrder);
        auto moveCandidates = generate_move_candidates(*activeKnight);

        bool moveWasAccepted = false;
        for (int attemptIndex = 0;
             attemptIndex < moveCandidateCount && !moveWasAccepted;
             attemptIndex += 1)
        {
            int attemptedMoveNumber = moveOrder[attemptIndex];
            const MoveCandidate &candidate = moveCandidates[attemptedMoveNumber - 1];
            int destinationRow = 0;
            int destinationColumn = 0;
            MoveOutcome outcome = tryMove(otherKnight, candidate, destinationRow,
                                          destinationColumn);

            if (outcome == MoveOutcome::outsideBoard)
            {
                printIllegalAttempt(attemptedMoveNumber, destinationRow,
                                    destinationColumn, "outside");
                continue;
            }
            if (outcome == MoveOutcome::visited)
            {
                printIllegalAttempt(attemptedMoveNumber, destinationRow,
                                    destinationColumn, "visited");
                continue;
            }
            if (outcome == MoveOutcome::capture)
            {
                printCaptureAttempt(attemptedMoveNumber, destinationRow,
                                    destinationColumn, otherKnightName);
                successfulMoveCount += 1;
                printMove(successfulMoveCount, activeKnight, destinationRow,
                          destinationColumn, true);
                board->mark_visited(destinationRow, destinationColumn);
                winnerName = activeKnightName;
                moveWasAccepted = true;
                continue;
            }

            printLegalAttempt(attemptedMoveNumber, destinationRow, destinationColumn);
            successfulMoveCount += 1;
            printMove(successfulMoveCount, activeKnight, destinationRow,
                      destinationColumn, false);
            board->mark_visited(destinationRow, destinationColumn);
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

    void playGame(bool playerIsActive, int &successfulMoveCount, std::string &winnerName)
    {
        while (winnerName.empty() && successfulMoveCount < maximumMoveCount)
        {
            playTurn(playerIsActive, successfulMoveCount, winnerName);
        }

        if (winnerName.empty())
        {
            winnerName = "draw";
        }
        std::printf("winner=%s\n", winnerName.c_str());
        std::printf("moves=%d\n", successfulMoveCount);
    }
};
