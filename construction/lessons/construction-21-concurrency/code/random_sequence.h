#pragma once

#include <cstdint>

class RandomSequence
{
public:
    void set_seed(std::int64_t seed);
    std::uint64_t below(std::uint64_t bound);
    int integer(int minimum, int maximum);

private:
    std::uint64_t state = 0;
};
