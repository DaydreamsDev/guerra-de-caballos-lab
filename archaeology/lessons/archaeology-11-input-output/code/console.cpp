#include "core.h"

#include <cstdio>

void print_result(const GameResult &result)
{
    for (const std::string &line : result.lines)
    {
        std::printf("%s\n", line.c_str());
    }
}
