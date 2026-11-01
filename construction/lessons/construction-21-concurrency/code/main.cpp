#include "game.h"

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace
{
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
}

int main(int argumentCount, char **argumentValues)
{
    std::int64_t seedValue = 0;
    std::string playerName;
    if (!parseArguments(argumentCount, argumentValues, seedValue, playerName))
    {
        return 2;
    }
    return run_game(seedValue, playerName);
}
