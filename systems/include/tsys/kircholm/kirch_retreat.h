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
    void prepare() noexcept;
    bool is_done();

    void process_retreats();
    void defence_finished() noexcept;
    void attack_finished() noexcept;

   private:
    bool fetch_retreat();

    std::vector<DirectFightResult> _results{};
    std::optional<DirectFightResult> _current{};
    bool _deffenders_done{true};
    bool _attackers_done{true};
    bool _vec_not_set{false};
};
}  // namespace kirch

#endif /* KIRCH_RETREAT_H */
