#ifndef KIRCH_SYSTEM_H
#define KIRCH_SYSTEM_H

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>

#include "toc/gameplay/system_state.h"

#include "kirch_mover.h"

namespace kirch {
class SystemKircholm : public SystemState {
   public:
    SystemKircholm();
    virtual ~SystemKircholm() = default;

    virtual void start() override;
    virtual void next_phase() override;
    virtual void prepare_lua(sol::state& lua) override;
    virtual void handle_hex_over(const HexCoordinate& hex) override;
    virtual void handle_hex_selection(const HexCoordinate& hex) override;
    virtual void handle_hex_info(const HexCoordinate& hex) override;
    virtual void handle_hex_release(const HexCoordinate& hex) override;
    virtual std::shared_ptr<DebugInfoSystem> create_debug_info() override;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    enum class StatePhase { not_started, movement, bombardment, attack, counterattack };
    StatePhase _current_phase{StatePhase::not_started};

    std::shared_ptr<DebugInfoSystem> _debug{nullptr};
    MovementSystem _movement;
    std::optional<HexCoordinate> _tmp_move_target{};
};

template <class Archive>
void SystemKircholm::serialize(Archive& archive) {
    archive(cereal::virtual_base_class<SystemState>(this),
            // list all components
            cereal::make_nvp("MovementComponents",
                             *scenario->units.get_container<MovementComponent>()),
            cereal::make_nvp("DirectFightComponents",
                             *scenario->units.get_container<DirectFightComponent>()),
            // end of list
            CEREAL_NVP(_current_phase));
}
}  // namespace kirch

#endif