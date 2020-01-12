#include "kircholm/kirch_actions.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(SystemAction<kirch::SystemKircholm>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, SystemAction<kirch::SystemKircholm>);

namespace kirch {

DirectFightResultsComputed::DirectFightResultsComputed(
    const std::vector<DirectFightResult>& results)
    : _results{results} {
    _results.erase(std::remove_if(_results.begin(), _results.end(),
                                  [](const auto& val) { return val.break_through == 0; }),
                   _results.end());
}

bool DirectFightResultsComputed::sys_execute(SystemKircholm* system) {
    system->retreat.set_results(to_insert);
    return true;
}

bool DirectFightResultsComputed::sys_revert(SystemKircholm*) { return false; }

DirectFightResultChanged::DirectFightResultChanged(
    const std::vector<DirectFightResult>& results, int index)
    : _result{results.at(index)}, _index{index} {}

bool DirectFightResultChanged::sys_execute(SystemKircholm* system) {
    system->retreat.set_result_change(_result, _index);
    return true;
}

bool DirectFightResultChanged::sys_revert(SystemKircholm* system) { return false; }

RetretsPerformed::RetretsPerformed(const std::vector<DirectFightResult>& results)
    : _results{results} {}

bool RetretsPerformed::sys_execute(SystemKircholm* system) {
    system->chase.set_results(_results);
    return true;
}

bool RetretsPerformed::sys_revert(SystemKircholm* system) { return false; }

}  // namespace kirch

CEREAL_REGISTER_TYPE(kirch::DirectFightResultsComputed);
CEREAL_REGISTER_POLYMORPHIC_RELATION(SystemAction<kirch::SystemKircholm>,
                                     kirch::DirectFightResultsComputed);

CEREAL_REGISTER_TYPE(kirch::DirectFightResultChanged);
CEREAL_REGISTER_POLYMORPHIC_RELATION(SystemAction<kirch::SystemKircholm>,
                                     kirch::DirectFightResultChanged);

CEREAL_REGISTER_TYPE(kirch::RetretsPerformed);
CEREAL_REGISTER_POLYMORPHIC_RELATION(SystemAction<kirch::SystemKircholm>,
                                     kirch::RetretsPerformed);