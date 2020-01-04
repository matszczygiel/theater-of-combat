#ifndef KIRCH_RETREAT_H
#define KIRCH_RETREAT_H

#include <optional>
#include <vector>

#include "kirch_component_system.h"
#include "kirch_fight.h"

namespace kirch {

class RetreatSystem : public ComponentSystemKircholm {
   public:
    explicit RetreatSystem(SystemKircholm* system) noexcept;
    void set_results(const std::vector<DirectFightResult>& results);
    void prepare_for_retreating() noexcept;
    bool is_done();

    void process_retreats();

   private:
    bool fetch_retreat();

    std::vector<DirectFightResult> _results{};
    std::optional<DirectFightResult> _current{};
    bool _prepared{false};
};
}  // namespace kirch

#endif /* KIRCH_RETREAT_H */
