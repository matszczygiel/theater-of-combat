#ifndef KIRCH_FIGHT_H
#define KIRCH_FIGHT_H

#include <array>
#include <set>

#include "toc/gameplay/action.h"
#include "toc/gameplay/scenario.h"
#include "toc/gameplay/system_state.h"
#include "toc/unit/unit.h"

namespace kirch {
struct DirectFightResult {
    
};

struct DirectFightData {
    std::array<std::set<Unit::IdType>, 2> ids{};
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

    DirectFightResult process_fight(const DirectFightData& data) const;
};
}  // namespace kirch

#endif /* KIRCH_FIGHT_H */
