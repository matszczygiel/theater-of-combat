#ifndef KIRCH_ACTIONS_H
#define KIRCH_ACTIONS_H

#include <cereal/types/vector.hpp>

#include "toc/gameplay/actions.h"

#include "kirch_fight.h"
#include "kirch_system.h"

namespace kirch {
class DirectFightResultComputed : public SystemAction<SystemKircholm> {
   public:
    DirectFightResultComputed() = default;
    explicit DirectFightResultComputed(const std::vector<DirectFightResult>& results);
    bool sys_execute(SystemKircholm* system) override;
    bool sys_revert(SystemKircholm* system) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    std::vector<DirectFightResult> _results{};
};

template <class Archive>
void DirectFightResultComputed::serialize(Archive& ar) {
    ar(CEREAL_NVP(_results));
}

}  // namespace kirch
#endif /* KIRCH_ACTIONS_H */
