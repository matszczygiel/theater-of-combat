#include "randomizer.h"

#include <chrono>

std::mt19937 Randomizer::_engine(std::chrono::high_resolution_clock::now()
                                   .time_since_epoch()
                                   .count());

std::mt19937& Randomizer::get_engine() noexcept { return _engine; }
