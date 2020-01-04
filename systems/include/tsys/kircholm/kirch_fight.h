#ifndef KIRCH_FIGHT_H
#define KIRCH_FIGHT_H

#include <array>
#include <optional>
#include <set>

#include "toc/gameplay/actions.h"
#include "toc/gameplay/component_system.h"
#include "toc/gameplay/scenario.h"
#include "toc/gameplay/system.h"

#include "toc/unit/unit.h"

#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>

#include "kirch_components.h"

namespace kirch {
struct DirectFightData {
    std::set<Unit::IdType> attacker_units;
    std::set<Unit::IdType> deffender_units;

    template <class Archive>
    void serialize(Archive& ar);
};

template <class Archive>
void DirectFightData::serialize(Archive& ar) {
    ar(CEREAL_NVP(attacker_units), CEREAL_NVP(deffender_units));
}

struct DirectFightResult {
    std::set<Unit::IdType> units_destroyed;
    // does not cout destroyed
    std::map<Unit::IdType, Strength> losses;
    // negative value means attackers loose, 0 nobody wins
    int break_through;
    bool disorganisation;
    DirectFightData ids;

    template <class Archive>
    void serialize(Archive& ar);
};

template <class Archive>
void DirectFightResult::serialize(Archive& ar) {
    ar(CEREAL_NVP(units_destroyed), CEREAL_NVP(losses), CEREAL_NVP(break_through),
       CEREAL_NVP(disorganisation), CEREAL_NVP(ids));
}

class SystemKircholm;

class DirectFightSystem : public ComponentSystem {
   public:
    DirectFightSystem(SystemKircholm* system) noexcept;
    void init_direct_fights();

   private:
    // < unit -> site, site -> units controling >
    std::pair<std::map<Unit::IdType, HexCoordinate>,
              std::map<HexCoordinate, std::set<Unit::IdType>>>
    get_positions(const std::set<Unit::IdType>& units) const;

    std::vector<DirectFightData> generate_data_vec() const;
    Strength accumulate_strength(const std::set<Unit::IdType>& units) const;
    DirectFightResult process_fight(const DirectFightData& data) const;
    void send_actions(const DirectFightResult& res);
};
}  // namespace kirch

#endif /* KIRCH_FIGHT_H */
