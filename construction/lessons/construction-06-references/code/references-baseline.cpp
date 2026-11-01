#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

constexpr int boardRowCount = 8;
constexpr int boardColumnCount = 8;
constexpr int boardCellCount = boardRowCount * boardColumnCount;
constexpr int moveOrderMinimum = 1;
constexpr int moveOrderMaximum = 8;
constexpr int rollMinimum = 1;
constexpr int rollMaximum = 50;
constexpr int maximumMoveCount = 200;
constexpr std::uint64_t randomMultiplier = 6364136223846793005ULL;
constexpr std::uint64_t randomIncrement = 1442695040888963407ULL;

constexpr int knightRowChangePerMove[moveOrderMaximum] = {-1, -2, -2, -1, 1, 2, 2, 1};
constexpr int knightColumnChangePerMove[moveOrderMaximum] = {2, 1, -1, -2, -2, -1, 1, 2};

struct Knight
{
    int row;
    int column;
};

struct BoardCell
{
    bool visited;
};

enum class MoveOutcome
{
    outsideBoard,
    visited,
    capture,
    legal
};

std::uint64_t randomState = 0;

std::uint64_t nextRandomValue()
{
    randomState = randomState * randomMultiplier + randomIncrement;
    return randomState;
}

std::uint64_t randomBelow(std::uint64_t bound)
{
    return nextRandomValue() % bound;
}

int randomInteger(int minimum, int maximum)
{
    std::uint64_t rangeSize = static_cast<std::uint64_t>(maximum - minimum + 1);
    return minimum + static_cast<int>(randomBelow(rangeSize));
}

int convertCellNumberToRow(int cellNumber)
{
    return cellNumber / boardColumnCount + 1;
}

int convertCellNumberToColumn(int cellNumber)
{
    return cellNumber % boardColumnCount + 1;
}

bool parseArguments(int argumentCount, char **argumentValues, std::int64_t &seedValue,
                    std::string &playerName)
{
    bool seedWasProvided = false;
    bool nameWasProvided = false;

    for (int argumentIndex = 1; argumentIndex < argumentCount; argumentIndex += 1)
    {
        std::string argumentText = argumentValues[argumentIndex];
        if (argumentText == "--seed")
        {
            argumentIndex += 1;
            if (argumentIndex >= argumentCount)
            {
                std::fprintf(stderr, "missing value after --seed\n");
                return false;
            }
            char *seedParseEnd = nullptr;
            errno = 0;
            long long seedCandidate = std::strtoll(argumentValues[argumentIndex],
                                                   &seedParseEnd, 10);
            if (errno == ERANGE || seedParseEnd == argumentValues[argumentIndex] ||
                *seedParseEnd != '\0')
            {
                std::fprintf(stderr, "invalid integer after --seed\n");
                return false;
            }
            seedValue = static_cast<std::int64_t>(seedCandidate);
            seedWasProvided = true;
        }
        else if (argumentText == "--name")
        {
            argumentIndex += 1;
            if (argumentIndex >= argumentCount)
            {
                std::fprintf(stderr, "missing value after --name\n");
                return false;
            }
            playerName = argumentValues[argumentIndex];
            nameWasProvided = true;
        }
        else
        {
            std::fprintf(stderr, "unknown argument: %s\n", argumentText.c_str());
            return false;
        }
    }

    if (!seedWasProvided || !nameWasProvided)
    {
        std::fprintf(stderr, "usage: %s --seed INTEGER --name TEXT\n", argumentValues[0]);
        return false;
    }
    return true;
}

void initializePositions(Knight &playerKnight, Knight &machineKnight)
{
    int playerCellNumber = static_cast<int>(randomBelow(boardCellCount));
    int machineCellNumber = static_cast<int>(randomBelow(boardCellCount));
    while (machineCellNumber == playerCellNumber)
    {
        machineCellNumber = static_cast<int>(randomBelow(boardCellCount));
    }
    playerKnight.row = convertCellNumberToRow(playerCellNumber);
    playerKnight.column = convertCellNumberToColumn(playerCellNumber);
    machineKnight.row = convertCellNumberToRow(machineCellNumber);
    machineKnight.column = convertCellNumberToColumn(machineCellNumber);
}

void initializeRolls(int &playerRoll, int &machineRoll)
{
    playerRoll = randomInteger(rollMinimum, rollMaximum);
    machineRoll = randomInteger(rollMinimum, rollMaximum);
    while (machineRoll == playerRoll)
    {
        machineRoll = randomInteger(rollMinimum, rollMaximum);
    }
}

void printGameHeader(std::int64_t seedValue, const std::string &playerName, int playerRoll,
                     int machineRoll, bool playerIsActive, const Knight &playerKnight,
                     const Knight &machineKnight)
{
    std::printf("seed=%lld\n", static_cast<long long>(seedValue));
    std::printf("player=%s\n", playerName.c_str());
    std::printf("roll_player=%d\n", playerRoll);
    std::printf("roll_machine=%d\n", machineRoll);
    std::printf("start=%s\n", playerIsActive ? "player" : "machine");
    std::printf("initial_player=%d,%d\n", playerKnight.row, playerKnight.column);
    std::printf("initial_machine=%d,%d\n", machineKnight.row, machineKnight.column);
}

void buildMoveOrder(int startingMoveNumber, int moveOrder[moveOrderMaximum])
{
    for (int orderIndex = 0; orderIndex < moveOrderMaximum; orderIndex += 1)
    {
        moveOrder[orderIndex] =
            (startingMoveNumber - 1 + orderIndex) % moveOrderMaximum + 1;
    }
}

void printTurnHeader(int turnNumber, const char *activeKnightName, const Knight &activeKnight,
                     const int moveOrder[moveOrderMaximum])
{
    std::printf("turn=%d who=%s at=%d,%d order=", turnNumber, activeKnightName,
                activeKnight.row, activeKnight.column);
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

void printMove(int moveNumber, const char *knightName, const Knight &fromKnight,
               int toRow, int toColumn, bool wasCapture)
{
    std::printf("move=%d %s %d,%d->%d,%d", moveNumber, knightName, fromKnight.row,
                fromKnight.column, toRow, toColumn);
    if (wasCapture)
    {
        std::printf(" capture");
    }
    std::printf("\n");
}

Knight &selectActiveKnight(bool playerIsActive, Knight &playerKnight, Knight &machineKnight)
{
    return playerIsActive ? playerKnight : machineKnight;
}

const Knight &selectOtherKnight(bool playerIsActive, const Knight &playerKnight,
                                const Knight &machineKnight)
{
    return playerIsActive ? machineKnight : playerKnight;
}

void moveKnight(Knight &knight, int destinationRow, int destinationColumn)
{
    knight.row = destinationRow;
    knight.column = destinationColumn;
}

MoveOutcome tryMove(const Knight &activeKnight, const Knight &otherKnight, int moveNumber,
                    BoardCell boardCells[][boardColumnCount + 1], int &destinationRow,
                    int &destinationColumn)
{
    destinationRow = activeKnight.row + knightRowChangePerMove[moveNumber - 1];
    destinationColumn = activeKnight.column + knightColumnChangePerMove[moveNumber - 1];

    if (destinationRow < 1 || destinationRow > boardRowCount ||
        destinationColumn < 1 || destinationColumn > boardColumnCount)
    {
        return MoveOutcome::outsideBoard;
    }
    if (destinationRow == otherKnight.row && destinationColumn == otherKnight.column)
    {
        return MoveOutcome::capture;
    }
    if (boardCells[destinationRow][destinationColumn].visited)
    {
        return MoveOutcome::visited;
    }
    return MoveOutcome::legal;
}

void playTurn(bool &playerIsActive, Knight &playerKnight, Knight &machineKnight,
              BoardCell boardCells[][boardColumnCount + 1], int &successfulMoveCount,
              std::string &winnerName)
{
    int startingMoveNumber = randomInteger(moveOrderMinimum, moveOrderMaximum);
    int moveOrder[moveOrderMaximum] = {};
    buildMoveOrder(startingMoveNumber, moveOrder);

    const char *activeKnightName = playerIsActive ? "player" : "machine";
    const char *otherKnightName = playerIsActive ? "machine" : "player";
    Knight &activeKnight = selectActiveKnight(playerIsActive, playerKnight, machineKnight);
    const Knight &otherKnight =
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
                                      boardCells, destinationRow, destinationColumn);

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
            boardCells[destinationRow][destinationColumn].visited = true;
            winnerName = activeKnightName;
            moveWasAccepted = true;
            continue;
        }

        printLegalAttempt(attemptedMoveNumber, destinationRow, destinationColumn);
        successfulMoveCount += 1;
        printMove(successfulMoveCount, activeKnightName, activeKnight, destinationRow,
                  destinationColumn, false);
        boardCells[destinationRow][destinationColumn].visited = true;
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

void playGame(Knight &playerKnight, Knight &machineKnight, bool playerIsActive,
              BoardCell boardCells[][boardColumnCount + 1], int &successfulMoveCount,
              std::string &winnerName)
{
    while (winnerName.empty() && successfulMoveCount < maximumMoveCount)
    {
        playTurn(playerIsActive, playerKnight, machineKnight, boardCells,
                 successfulMoveCount, winnerName);
    }

    if (winnerName.empty())
    {
        winnerName = "draw";
    }
    std::printf("winner=%s\n", winnerName.c_str());
    std::printf("moves=%d\n", successfulMoveCount);
}

int main(int argumentCount, char **argumentValues)
{
    std::int64_t seedValue = 0;
    std::string playerName;
    if (!parseArguments(argumentCount, argumentValues, seedValue, playerName))
    {
        return 2;
    }

    randomState = static_cast<std::uint64_t>(seedValue);

    Knight playerKnight = {};
    Knight machineKnight = {};
    initializePositions(playerKnight, machineKnight);

    int playerRoll = 0;
    int machineRoll = 0;
    initializeRolls(playerRoll, machineRoll);

    bool playerIsActive = playerRoll > machineRoll;
    printGameHeader(seedValue, playerName, playerRoll, machineRoll, playerIsActive,
                    playerKnight, machineKnight);

    BoardCell boardCells[boardRowCount + 1][boardColumnCount + 1] = {};
    int successfulMoveCount = 0;
    std::string winnerName;
    playGame(playerKnight, machineKnight, playerIsActive, boardCells, successfulMoveCount,
             winnerName);
    return 0;
}
