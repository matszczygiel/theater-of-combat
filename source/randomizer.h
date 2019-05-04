#include <random>

class Randomizer {
   public:
    Randomizer() = delete;

    static std::mt19937& get_engine() noexcept;

   private:
    static std::mt19937 _engine;
};
