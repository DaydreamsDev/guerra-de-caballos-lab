#pragma once

#include <cstdint>

void random_set_seed(std::int64_t seed);
std::uint64_t random_below(std::uint64_t bound);
int random_integer(int minimum, int maximum);
