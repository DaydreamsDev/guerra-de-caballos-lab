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

void initializePositions(int &playerRow, int &playerColumn, int &machineRow,
                         int &machineColumn)
{
    int playerCellNumber = static_cast<int>(randomBelow(boardCellCount));
    int machineCellNumber = static_cast<int>(randomBelow(boardCellCount));
    while (machineCellNumber == playerCellNumber)
    {
        machineCellNumber = static_cast<int>(randomBelow(boardCellCount));
    }
    playerRow = convertCellNumberToRow(playerCellNumber);
    playerColumn = convertCellNumberToColumn(playerCellNumber);
    machineRow = convertCellNumberToRow(machineCellNumber);
    machineColumn = convertCellNumberToColumn(machineCellNumber);
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
                     int machineRoll, bool playerIsActive, int playerRow, int playerColumn,
                     int machineRow, int machineColumn)
{
    std::printf("seed=%lld\n", static_cast<long long>(seedValue));
    std::printf("player=%s\n", playerName.c_str());
    std::printf("roll_player=%d\n", playerRoll);
    std::printf("roll_machine=%d\n", machineRoll);
    std::printf("start=%s\n", playerIsActive ? "player" : "machine");
    std::printf("initial_player=%d,%d\n", playerRow, playerColumn);
    std::printf("initial_machine=%d,%d\n", machineRow, machineColumn);
}

void buildMoveOrder(int startingMoveNumber, int moveOrder[moveOrderMaximum])
{
    for (int orderIndex = 0; orderIndex < moveOrderMaximum; orderIndex += 1)
    {
        moveOrder[orderIndex] =
            (startingMoveNumber - 1 + orderIndex) % moveOrderMaximum + 1;
    }
}

void printTurnHeader(int turnNumber, const char *activeKnightName, int activeKnightRow,
                     int activeKnightColumn, const int moveOrder[moveOrderMaximum])
{
    std::printf("turn=%d who=%s at=%d,%d order=", turnNumber, activeKnightName,
                activeKnightRow, activeKnightColumn);
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

void printMove(int moveNumber, const char *knightName, int fromRow, int fromColumn,
               int toRow, int toColumn, bool wasCapture)
{
    std::printf("move=%d %s %d,%d->%d,%d", moveNumber, knightName, fromRow, fromColumn,
                toRow, toColumn);
    if (wasCapture)
    {
        std::printf(" capture");
    }
    std::printf("\n");
}

MoveOutcome tryMove(int activeKnightRow, int activeKnightColumn, int otherKnightRow,
                    int otherKnightColumn, int moveNumber,
                    bool visitedCells[][boardColumnCount + 1], int &destinationRow,
                    int &destinationColumn)
{
    destinationRow = activeKnightRow + knightRowChangePerMove[moveNumber - 1];
    destinationColumn = activeKnightColumn + knightColumnChangePerMove[moveNumber - 1];

    if (destinationRow < 1 || destinationRow > boardRowCount ||
        destinationColumn < 1 || destinationColumn > boardColumnCount)
    {
        return MoveOutcome::outsideBoard;
    }
    if (destinationRow == otherKnightRow && destinationColumn == otherKnightColumn)
    {
        return MoveOutcome::capture;
    }
    if (visitedCells[destinationRow][destinationColumn])
    {
        return MoveOutcome::visited;
    }
    return MoveOutcome::legal;
}

void playTurn(bool &playerIsActive, int &playerRow, int &playerColumn, int &machineRow,
              int &machineColumn, bool visitedCells[][boardColumnCount + 1],
              int &successfulMoveCount, std::string &winnerName)
{
    int startingMoveNumber = randomInteger(moveOrderMinimum, moveOrderMaximum);
    int moveOrder[moveOrderMaximum] = {};
    buildMoveOrder(startingMoveNumber, moveOrder);

    const char *activeKnightName = playerIsActive ? "player" : "machine";
    const char *otherKnightName = playerIsActive ? "machine" : "player";
    int activeKnightRow = playerIsActive ? playerRow : machineRow;
    int activeKnightColumn = playerIsActive ? playerColumn : machineColumn;
    int otherKnightRow = playerIsActive ? machineRow : playerRow;
    int otherKnightColumn = playerIsActive ? machineColumn : playerColumn;

    printTurnHeader(successfulMoveCount + 1, activeKnightName, activeKnightRow,
                    activeKnightColumn, moveOrder);

    bool moveWasAccepted = false;
    for (int attemptIndex = 0;
         attemptIndex < moveOrderMaximum && !moveWasAccepted;
         attemptIndex += 1)
    {
        int attemptedMoveNumber = moveOrder[attemptIndex];
        int destinationRow = 0;
        int destinationColumn = 0;
        MoveOutcome outcome = tryMove(activeKnightRow, activeKnightColumn, otherKnightRow,
                                      otherKnightColumn, attemptedMoveNumber, visitedCells,
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
            printMove(successfulMoveCount, activeKnightName, activeKnightRow,
                      activeKnightColumn, destinationRow, destinationColumn, true);
            visitedCells[destinationRow][destinationColumn] = true;
            winnerName = activeKnightName;
            moveWasAccepted = true;
            continue;
        }

        printLegalAttempt(attemptedMoveNumber, destinationRow, destinationColumn);
        successfulMoveCount += 1;
        printMove(successfulMoveCount, activeKnightName, activeKnightRow, activeKnightColumn,
                  destinationRow, destinationColumn, false);
        visitedCells[destinationRow][destinationColumn] = true;
        if (playerIsActive)
        {
            playerRow = destinationRow;
            playerColumn = destinationColumn;
        }
        else
        {
            machineRow = destinationRow;
            machineColumn = destinationColumn;
        }
        playerIsActive = !playerIsActive;
        moveWasAccepted = true;
    }

    if (!moveWasAccepted)
    {
        std::printf("  no_legal_move trapped=%s\n", activeKnightName);
        winnerName = otherKnightName;
    }
}

void playGame(int &playerRow, int &playerColumn, int &machineRow, int &machineColumn,
              bool playerIsActive, bool visitedCells[][boardColumnCount + 1],
              int &successfulMoveCount, std::string &winnerName)
{
    while (winnerName.empty() && successfulMoveCount < maximumMoveCount)
    {
        playTurn(playerIsActive, playerRow, playerColumn, machineRow, machineColumn,
                 visitedCells, successfulMoveCount, winnerName);
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

    int playerRow = 0;
    int playerColumn = 0;
    int machineRow = 0;
    int machineColumn = 0;
    initializePositions(playerRow, playerColumn, machineRow, machineColumn);

    int playerRoll = 0;
    int machineRoll = 0;
    initializeRolls(playerRoll, machineRoll);

    bool playerIsActive = playerRoll > machineRoll;
    printGameHeader(seedValue, playerName, playerRoll, machineRoll, playerIsActive,
                    playerRow, playerColumn, machineRow, machineColumn);

    bool visitedCells[boardRowCount + 1][boardColumnCount + 1] = {};
    int successfulMoveCount = 0;
    std::string winnerName;
    playGame(playerRow, playerColumn, machineRow, machineColumn, playerIsActive, visitedCells,
             successfulMoveCount, winnerName);
    return 0;
}
