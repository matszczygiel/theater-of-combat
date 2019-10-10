#include "core/randomize.h"

namespace randomize {

std::mt19937& engine() noexcept {
    static std::random_device rd{};
    static std::mt19937 engine{rd()};
    return engine;
}

}  // namespace randomize