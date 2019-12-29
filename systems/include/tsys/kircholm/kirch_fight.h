#ifndef KIRCH_FIGHT_H
#define KIRCH_FIGHT_H

#include <array>
#include <set>

#include "toc/gameplay/scenario.h"
#include "toc/gameplay/system_state.h"
#include "toc/unit/unit.h"

namespace kirch {
struct DirectFightResult {};

class FightSystem {
   public:
    FightSystem(const std::shared_ptr<Scenario>& scenario, SystemState* system) noexcept;

    std::vector<DirectFightResult> process_fights() const;

   private:
    std::shared_ptr<Scenario> _scenario{nullptr};
    SystemState* _system{nullptr};

    std::set<HexCoordinate> get_control_zone_of(
        const std::set<Unit::IdType>& units) const;
    std::set<Unit::IdType> find_units_in_zone(const std::set<Unit::IdType>& units,
                                              const std::set<HexCoordinate>& zone) const;
    // < unit -> site, site -> units controling >
    std::pair<std::map<Unit::IdType, HexCoordinate>,
              std::map<HexCoordinate, std::set<Unit::IdType>>>
    get_positions(const std::set<Unit::IdType>& units) const;
};
}  // namespace kirch

#endif /* KIRCH_FIGHT_H */
