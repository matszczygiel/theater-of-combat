#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <random>

namespace randomizer {

std::mt19937& engine() noexcept;
int uniform_int(const int& min, const int& max);

}  // namespace randomizer

#endif