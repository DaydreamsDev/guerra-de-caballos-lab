#include "random_sequence.h"

namespace
{
constexpr std::uint64_t randomMultiplier = 6364136223846793005ULL;
constexpr std::uint64_t randomIncrement = 1442695040888963407ULL;
std::uint64_t randomState = 0;
}

void random_set_seed(std::int64_t seed)
{
    randomState = static_cast<std::uint64_t>(seed);
}

std::uint64_t random_below(std::uint64_t bound)
{
    randomState = randomState * randomMultiplier + randomIncrement;
    return randomState % bound;
}

int random_integer(int minimum, int maximum)
{
    std::uint64_t rangeSize = static_cast<std::uint64_t>(maximum - minimum + 1);
    return minimum + static_cast<int>(random_below(rangeSize));
}
