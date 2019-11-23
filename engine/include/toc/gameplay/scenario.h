#ifndef SCENARIO_H
#define SCENARIO_H

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <sol/state.hpp>

#include "toc/core/log.h"
#include "toc/map/map.h"
#include "toc/unit/unit.h"
#include "toc/unit/unit_manager.h"

struct ScenarioDescription {
    std::string name{};
    std::string description{};
    std::string system{};
    std::string map_name{};
    std::string filename{};

    template <class Archive>
    void serialize(Archive& archive);
};

class Scenario {
   public:
    std::map<std::string, std::set<Unit::IdType>> teams{};
    UnitManager units{};
    Map map{};
    std::array<std::vector<std::string>, 2> player_teams{};

    bool load_script(sol::state& lua, const std::string& script);

    void next_turn(sol::state& lua);
    int current_turn() const;

    template <class Archive>
    void serialize(Archive& archive);

    bool prepare_lua_state(sol::state& lua) const;

   private:
    int _current_turn{0};
    std::string _script{};
};

template <class Archive>
void ScenarioDescription::serialize(Archive& archive) {
    archive(CEREAL_NVP(name), CEREAL_NVP(description), CEREAL_NVP(system),
            CEREAL_NVP(map_name), CEREAL_NVP(filename));
}

template <class Archive>
void Scenario::serialize(Archive& archive) {
    archive(CEREAL_NVP(teams), CEREAL_NVP(units), CEREAL_NVP(map),
            CEREAL_NVP(player_teams), CEREAL_NVP(_current_turn), CEREAL_NVP(_script));
}

#endif