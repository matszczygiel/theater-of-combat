#ifndef KIRCH_SYSTEM_H
#define KIRCH_SYSTEM_H

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>

#include "toc/gameplay/system.h"

#include "kirch_fight.h"
#include "kirch_mover.h"
#include "kirch_organization.h"
#include "kirch_retreat.h"
#include "kirch_chase.h"

namespace kirch {
class SystemKircholm : public System {
   public:
    SystemKircholm();
    virtual ~SystemKircholm() = default;

    virtual void start() override;
    virtual void next_phase() override;
    virtual void prepare_lua(sol::state& lua) override;

    virtual void on_over(const HexCoordinate& hex) override;
    virtual void on_left_click(const HexCoordinate& hex) override;
    virtual void on_right_click(const HexCoordinate& hex) override;
    virtual void on_left_realease(const HexCoordinate& hex) override;
    virtual void on_right_realease(const HexCoordinate& hex) override;

    virtual std::shared_ptr<DebugInfoSystem> create_debug_info() override;
    virtual void update_system() override;

    template <class Archive>
    void serialize(Archive& archive);

    enum class StatePhase { not_started, movement, bombardment, attack, counterattack };
    StatePhase current_phase{StatePhase::not_started};

    std::shared_ptr<DebugInfoSystem> debug{nullptr};

    MovementSystem movement;
    DirectFightSystem direct_fight;
    RetreatSystem retreat;
    OrganizationSystem organization;
    ChaseSystem chase;
};

template <class Archive>
void SystemKircholm::serialize(Archive& archive) {
    archive(cereal::virtual_base_class<System>(this),
            // list all components
            cereal::make_nvp("MovementComponents",
                             *scenario->units.get_container<MovementComponent>()),
            cereal::make_nvp("DirectFightComponents",
                             *scenario->units.get_container<DirectFightComponent>()),
            // end of list
            CEREAL_NVP(current_phase));
}
}  // namespace kirch

#endif