#include <array>
#include <cstdio>
#include <string>

namespace
{
bool loop_bound_test()
{
    std::array<int, 65> values = {};
    values[64] = 7;
    for (int index = 0; index <= 64; index += 1)
    {
        values[index] = 0;
    }
    return values[64] == 0;
}

bool seed_in_loop_test()
{
    unsigned int state = 0;
    auto seed = [&state](unsigned int value)
    {
        state = value;
    };
    auto next = [&state]()
    {
        state = state * 1664525U + 1013904223U;
        return state;
    };

    seed(1);
    unsigned int firstValue = next();
    bool repeated = false;
    for (int attempt = 0; attempt < 3; attempt += 1)
    {
        seed(1);
        if (next() == firstValue)
        {
            repeated = true;
        }
    }
    return repeated;
}

bool broken_initialization_test()
{
    std::array<int, 8> values = {1, 1, 1, 1, 1, 1, 1, 1};
    int *pointer = values.data();
    for (int index = 0; index < 8; index += 1)
    {
        *pointer = 0;
        pointer = values.data();
    }
    return values[0] == 0 && values[1] == 1;
}

std::string precursor_winner(bool allPawnsEaten)
{
    return allPawnsEaten ? "machine" : "player";
}

bool inverted_win_test()
{
    return precursor_winner(true) != "player";
}
}

int main()
{
    const char *names[] = {"failure-001", "failure-003", "failure-004", "failure-005"};
    bool results[] = {loop_bound_test(), seed_in_loop_test(),
                      broken_initialization_test(), inverted_win_test()};

    bool allDetected = true;
    for (int index = 0; index < 4; index += 1)
    {
        std::printf("%s=%s\n", names[index], results[index] ? "detected" : "not-detected");
        allDetected = allDetected && results[index];
    }
    return allDetected ? 0 : 1;
}
