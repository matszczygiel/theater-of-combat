#ifndef KIRCH_RETREAT_H
#define KIRCH_RETREAT_H

#include <vector>

#include "kirch_component_system.h"
#include "kirch_fight.h"

namespace kirch {

class RetreatSystem : public ComponentSystemKircholm {
   public:
    explicit RetreatSystem(SystemKircholm* system) noexcept;
    void set_results(const std::vector<DirectFightResult>& results);
    bool is_done();

    void process_retreats();

   private:
    std::vector<DirectFightResult> _results{};
};
}  // namespace kirch

#endif /* KIRCH_RETREAT_H */
