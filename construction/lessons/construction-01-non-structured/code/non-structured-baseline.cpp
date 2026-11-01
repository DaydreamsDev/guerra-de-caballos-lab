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

std::uint64_t randomState = 0;
std::int64_t seedValue = 0;
std::string playerName;
bool seedWasProvided = false;
bool nameWasProvided = false;
char *seedParseEnd = nullptr;

int playerCellNumber = 0;
int machineCellNumber = 0;
int playerRow = 0;
int playerColumn = 0;
int machineRow = 0;
int machineColumn = 0;
int playerRoll = 0;
int machineRoll = 0;
bool playerIsActive = false;
std::string winnerName;
bool visitedCells[boardRowCount + 1][boardColumnCount + 1] = {};

int startingMoveNumber = 0;
int moveOrder[moveOrderMaximum] = {};
int attemptIndex = 0;
int attemptedMoveNumber = 0;
int successfulMoveCount = 0;
int activeKnightRow = 0;
int activeKnightColumn = 0;
int otherKnightRow = 0;
int otherKnightColumn = 0;
int destinationRow = 0;
int destinationColumn = 0;
bool moveWasAccepted = false;
const char *activeKnightName = "player";
const char *otherKnightName = "machine";

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

int main(int argumentCount, char **argumentValues)
{
    for (int argumentIndex = 1; argumentIndex < argumentCount; argumentIndex += 1)
    {
        std::string argumentText = argumentValues[argumentIndex];
        if (argumentText == "--seed")
        {
            argumentIndex += 1;
            if (argumentIndex >= argumentCount)
            {
                std::fprintf(stderr, "missing value after --seed\n");
                return 2;
            }
            seedParseEnd = nullptr;
            errno = 0;
            long long seedCandidate = std::strtoll(argumentValues[argumentIndex],
                                                   &seedParseEnd, 10);
            if (errno == ERANGE || seedParseEnd == argumentValues[argumentIndex] ||
                *seedParseEnd != '\0')
            {
                std::fprintf(stderr, "invalid integer after --seed\n");
                return 2;
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
                return 2;
            }
            playerName = argumentValues[argumentIndex];
            nameWasProvided = true;
        }
        else
        {
            std::fprintf(stderr, "unknown argument: %s\n", argumentText.c_str());
            return 2;
        }
    }

    if (!seedWasProvided || !nameWasProvided)
    {
        std::fprintf(stderr, "usage: %s --seed INTEGER --name TEXT\n", argumentValues[0]);
        return 2;
    }

    randomState = static_cast<std::uint64_t>(seedValue);

    playerCellNumber = static_cast<int>(randomBelow(boardCellCount));
    machineCellNumber = static_cast<int>(randomBelow(boardCellCount));
    while (machineCellNumber == playerCellNumber)
    {
        machineCellNumber = static_cast<int>(randomBelow(boardCellCount));
    }
    playerRow = convertCellNumberToRow(playerCellNumber);
    playerColumn = convertCellNumberToColumn(playerCellNumber);
    machineRow = convertCellNumberToRow(machineCellNumber);
    machineColumn = convertCellNumberToColumn(machineCellNumber);

    playerRoll = randomInteger(rollMinimum, rollMaximum);
    machineRoll = randomInteger(rollMinimum, rollMaximum);
    while (machineRoll == playerRoll)
    {
        machineRoll = randomInteger(rollMinimum, rollMaximum);
    }

    playerIsActive = playerRoll > machineRoll;

    std::printf("seed=%lld\n", static_cast<long long>(seedValue));
    std::printf("player=%s\n", playerName.c_str());
    std::printf("roll_player=%d\n", playerRoll);
    std::printf("roll_machine=%d\n", machineRoll);
    std::printf("start=%s\n", playerIsActive ? "player" : "machine");
    std::printf("initial_player=%d,%d\n", playerRow, playerColumn);
    std::printf("initial_machine=%d,%d\n", machineRow, machineColumn);

gameLoopStart:
    if (!winnerName.empty() || successfulMoveCount >= maximumMoveCount)
    {
        goto gameLoopEnd;
    }

    startingMoveNumber = randomInteger(moveOrderMinimum, moveOrderMaximum);
    for (int orderIndex = 0; orderIndex < moveOrderMaximum; orderIndex += 1)
    {
        moveOrder[orderIndex] =
            (startingMoveNumber - 1 + orderIndex) % moveOrderMaximum + 1;
    }

    if (playerIsActive)
    {
        activeKnightName = "player";
        otherKnightName = "machine";
        activeKnightRow = playerRow;
        activeKnightColumn = playerColumn;
        otherKnightRow = machineRow;
        otherKnightColumn = machineColumn;
    }
    else
    {
        activeKnightName = "machine";
        otherKnightName = "player";
        activeKnightRow = machineRow;
        activeKnightColumn = machineColumn;
        otherKnightRow = playerRow;
        otherKnightColumn = playerColumn;
    }

    std::printf("turn=%d who=%s at=%d,%d order=", successfulMoveCount + 1,
                activeKnightName, activeKnightRow, activeKnightColumn);
    for (int orderIndex = 0; orderIndex < moveOrderMaximum; orderIndex += 1)
    {
        std::printf("%d", moveOrder[orderIndex]);
        if (orderIndex + 1 < moveOrderMaximum)
        {
            std::printf(",");
        }
    }
    std::printf("\n");

    attemptIndex = 0;
    moveWasAccepted = false;
    goto attemptStart;

attemptStart:
    if (attemptIndex >= moveOrderMaximum)
    {
        goto noLegalMove;
    }
    attemptedMoveNumber = moveOrder[attemptIndex];
    destinationRow =
        activeKnightRow + knightRowChangePerMove[attemptedMoveNumber - 1];
    destinationColumn =
        activeKnightColumn + knightColumnChangePerMove[attemptedMoveNumber - 1];

    if (destinationRow < 1 || destinationRow > boardRowCount ||
        destinationColumn < 1 || destinationColumn > boardColumnCount)
    {
        goto attemptOutside;
    }
    if (destinationRow == otherKnightRow &&
        destinationColumn == otherKnightColumn)
    {
        goto attemptCapture;
    }
    if (visitedCells[destinationRow][destinationColumn])
    {
        goto attemptVisited;
    }
    goto attemptLegal;

attemptOutside:
    std::printf("  try=%d to=%d,%d illegal=outside\n", attemptedMoveNumber,
                destinationRow, destinationColumn);
    goto attemptAdvance;

attemptVisited:
    std::printf("  try=%d to=%d,%d illegal=visited\n", attemptedMoveNumber,
                destinationRow, destinationColumn);
    goto attemptAdvance;

attemptCapture:
    std::printf("  try=%d to=%d,%d capture=%s\n", attemptedMoveNumber,
                destinationRow, destinationColumn, otherKnightName);
    successfulMoveCount += 1;
    std::printf("move=%d %s %d,%d->%d,%d capture\n", successfulMoveCount,
                activeKnightName, activeKnightRow, activeKnightColumn,
                destinationRow, destinationColumn);
    visitedCells[destinationRow][destinationColumn] = true;
    winnerName = activeKnightName;
    moveWasAccepted = true;
    goto gameLoopStart;

attemptLegal:
    std::printf("  try=%d to=%d,%d legal\n", attemptedMoveNumber,
                destinationRow, destinationColumn);
    successfulMoveCount += 1;
    std::printf("move=%d %s %d,%d->%d,%d\n", successfulMoveCount,
                activeKnightName, activeKnightRow, activeKnightColumn,
                destinationRow, destinationColumn);
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
    goto gameLoopStart;

attemptAdvance:
    attemptIndex += 1;
    goto attemptStart;

noLegalMove:
    if (!moveWasAccepted)
    {
        std::printf("  no_legal_move trapped=%s\n", activeKnightName);
        winnerName = otherKnightName;
    }
    goto gameLoopStart;

gameLoopEnd:
    if (winnerName.empty())
    {
        winnerName = "draw";
    }
    std::printf("winner=%s\n", winnerName.c_str());
    std::printf("moves=%d\n", successfulMoveCount);
    return 0;
}
