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

RetreatsDone::RetreatsDone(bool send_by_local) noexcept : _send_by_local{send_by_local} {}

bool RetreatsDone::sys_execute(SystemKircholm* system) {
    if (_send_by_local)
        system->retreat.attack_finished();
    else
        system->retreat.defence_finished();
    return true;
}

bool RetreatsDone::sys_revert(SystemKircholm*) { return false; }

}  // namespace kirch

CEREAL_REGISTER_TYPE(kirch::DirectFightResultComputed);
CEREAL_REGISTER_POLYMORPHIC_RELATION(SystemAction<kirch::SystemKircholm>,
                                     kirch::DirectFightResultComputed);

CEREAL_REGISTER_TYPE(kirch::RetreatsDone);
CEREAL_REGISTER_POLYMORPHIC_RELATION(SystemAction<kirch::SystemKircholm>,
                                     kirch::RetreatsDone);