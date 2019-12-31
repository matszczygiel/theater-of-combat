#ifndef KIRCH_FIGHT_H
#define KIRCH_FIGHT_H

#include <array>
#include <set>

#include "toc/gameplay/action.h"
#include "toc/gameplay/scenario.h"
#include "toc/gameplay/system_state.h"
#include "toc/unit/unit.h"

#include "kirch_components.h"

namespace kirch {
struct DirectFightResult {
    std::set<Unit::IdType> units_destroyed;
    //does not cout destroyed
    std::map<Unit::IdType, Strength> losses;
    //negative value means attackers loose, 0 nobody wins
    int break_through;
};

struct DirectFightData {
    std::set<Unit::IdType> attacker_units;
    std::set<Unit::IdType> deffender_units;
};

class FightSystem {
   public:
    FightSystem(const std::shared_ptr<Scenario>& scenario, SystemState* system) noexcept;

    std::vector<DirectFightResult> process_fights() const;

   private:
    std::shared_ptr<Scenario> _scenario{nullptr};
    SystemState* _system{nullptr};

    // < unit -> site, site -> units controling >
    std::pair<std::map<Unit::IdType, HexCoordinate>,
              std::map<HexCoordinate, std::set<Unit::IdType>>>
    get_positions(const std::set<Unit::IdType>& units) const;

    std::vector<DirectFightData> generate_data_vec() const;
    Strength accumulate_strength(const std::set<Unit::IdType>& units) const;

    DirectFightResult process_fight(const DirectFightData& data) const;


};
}  // namespace kirch

#endif /* KIRCH_FIGHT_H */
