#ifndef RANDOMIZE_H
#define RANDOMIZE_H

#include <random>

namespace randomize {

std::mt19937& engine() noexcept;

}  // namespace randomize

#endif