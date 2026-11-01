#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace
{
constexpr std::uint64_t randomMultiplier = 6364136223846793005ULL;
constexpr std::uint64_t randomIncrement = 1442695040888963407ULL;

std::uint64_t randomState = 0;

void set_seed(int seed)
{
    randomState = static_cast<std::uint64_t>(seed);
}

std::uint64_t below(std::uint64_t bound)
{
    randomState = randomState * randomMultiplier + randomIncrement;
    return randomState % bound;
}
}

int main(int argumentCount, char **argumentValues)
{
    int seed = 1;
    if (argumentCount == 3 && std::strcmp(argumentValues[1], "--seed") == 0)
    {
        seed = std::atoi(argumentValues[2]);
    }

    set_seed(seed);
    int position = 0;
    std::printf("seed=%d\n", seed);
    for (int moveNumber = 1; moveNumber <= 3; moveNumber += 1)
    {
        position = (position + static_cast<int>(below(3)) + 1) % 8;
        std::printf("move=%d position=%d\n", moveNumber, position);
    }
    std::printf("winner=player\n");
    std::printf("moves=3\n");
    return 0;
}
