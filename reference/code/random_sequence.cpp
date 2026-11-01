#include "random_sequence.h"

namespace
{
constexpr std::uint64_t randomMultiplier = 6364136223846793005ULL;
constexpr std::uint64_t randomIncrement = 1442695040888963407ULL;
}

void RandomSequence::set_seed(std::int64_t seed)
{
    state = static_cast<std::uint64_t>(seed);
}

std::uint64_t RandomSequence::below(std::uint64_t bound)
{
    state = state * randomMultiplier + randomIncrement;
    return state % bound;
}

int RandomSequence::integer(int minimum, int maximum)
{
    std::uint64_t rangeSize = static_cast<std::uint64_t>(maximum - minimum + 1);
    return minimum + static_cast<int>(below(rangeSize));
}
