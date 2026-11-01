#pragma once

#include "board.h"
#include "knight.h"
#include "move_generator.h"
#include "random_sequence.h"
#include "transcript_port.h"

#include <array>
#include <concepts>
#include <cstdint>
#include <memory>
#include <ranges>
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
    explicit GameEngine(TranscriptPort &transcriptPort)
        : port(transcriptPort),
          board(std::make_unique<Board<RowCount, ColumnCount>>()),
          playerKnight(std::make_unique<Knight>("player")),
          machineKnight(std::make_unique<Knight>("machine"))
    {
    }

    GameEngine(const GameEngine &) = delete;
    GameEngine &operator=(const GameEngine &) = delete;
    GameEngine(GameEngine &&) = delete;
    GameEngine &operator=(GameEngine &&) = delete;

    int run(std::int64_t seed, const std::string &playerName)
    {
        random_set_seed(seed);
        board->reset();
        initializePositions();

        int playerRoll = 0;
        int machineRoll = 0;
        initializeRolls(playerRoll, machineRoll);

        bool playerIsActive = playerRoll > machineRoll;
        port.game_header(seed, playerName, playerRoll, machineRoll, playerIsActive,
                         playerKnight->row(), playerKnight->column(),
                         machineKnight->row(), machineKnight->column());

        int successfulMoveCount = 0;
        std::string winnerName;
        playGame(playerIsActive, successfulMoveCount, winnerName);
        return 0;
    }

private:
    TranscriptPort &port;
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

    void buildMoveOrder(int startingMoveNumber, int moveOrder[moveCandidateCount]) const
    {
        std::ranges::transform(
            std::views::iota(0, moveCandidateCount), moveOrder,
            [startingMoveNumber](int index)
            {
                return (startingMoveNumber - 1 + index) % moveCandidateCount + 1;
            });
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

        port.turn_header(successfulMoveCount + 1, activeKnightName, activeKnight->row(),
                         activeKnight->column(), moveOrder);
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
                port.illegal_attempt(attemptedMoveNumber, destinationRow, destinationColumn,
                                     "outside");
                continue;
            }
            if (outcome == MoveOutcome::visited)
            {
                port.illegal_attempt(attemptedMoveNumber, destinationRow, destinationColumn,
                                     "visited");
                continue;
            }
            if (outcome == MoveOutcome::capture)
            {
                port.capture_attempt(attemptedMoveNumber, destinationRow, destinationColumn,
                                     otherKnightName);
                successfulMoveCount += 1;
                port.move(successfulMoveCount, activeKnightName, activeKnight->row(),
                          activeKnight->column(), destinationRow, destinationColumn, true);
                board->mark_visited(destinationRow, destinationColumn);
                winnerName = activeKnightName;
                moveWasAccepted = true;
                continue;
            }

            port.legal_attempt(attemptedMoveNumber, destinationRow, destinationColumn);
            successfulMoveCount += 1;
            port.move(successfulMoveCount, activeKnightName, activeKnight->row(),
                      activeKnight->column(), destinationRow, destinationColumn, false);
            board->mark_visited(destinationRow, destinationColumn);
            moveKnight(activeKnight, destinationRow, destinationColumn);
            playerIsActive = !playerIsActive;
            moveWasAccepted = true;
        }

        if (!moveWasAccepted)
        {
            port.no_legal_move(activeKnightName);
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
        port.winner(winnerName.c_str(), successfulMoveCount);
    }
};
