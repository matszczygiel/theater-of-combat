#include "randomizer.h"

namespace randomizer {

std::mt19937& engine() noexcept {
    static std::random_device rd;
    static std::mt19937 engine{rd()};
    return engine;
}

int uniform_int(const int& min, const int& max) {
    return std::uniform_int_distribution{min, max}(engine());
}

}  // namespace random