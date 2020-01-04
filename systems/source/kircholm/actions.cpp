#include "kircholm/kirch_actions.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(SystemAction<kirch::SystemKircholm>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, SystemAction<kirch::SystemKircholm>);

namespace kirch {

DirectFightResultComputed::DirectFightResultComputed(
    const std::vector<DirectFightResult>& results)
    : _results{results} {}

bool DirectFightResultComputed::sys_execute(SystemKircholm* system) {
    const bool keep_negative = system->is_local_player_now();
    decltype(_results) to_insert;
    to_insert.reserve(_results.size());
    for (const auto& res : _results) {
        if (res.break_through == 0)
            continue;
        const bool neg = res.break_through < 0;
        if (keep_negative == neg) {
            to_insert.push_back(res);
        }
    }

    system->retreat.set_results(to_insert);
    return true;
}

bool DirectFightResultComputed::sys_revert(SystemKircholm*) { return false; }

}  // namespace kirch

CEREAL_REGISTER_TYPE(kirch::DirectFightResultComputed);
CEREAL_REGISTER_POLYMORPHIC_RELATION(SystemAction<kirch::SystemKircholm>,
                                     kirch::DirectFightResultComputed);