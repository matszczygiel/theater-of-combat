#ifndef KIRCHOLM_H
#define KIRCHOLM_H

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

#include "toc/gameplay/scenario.h"
#include "toc/gameplay/system_state.h"
#include "toc/gameplay/action.h"
#include "toc/map/hexagons.h"
#include "toc/unit/unit_components.h"

namespace kirch {

/// Basic types
enum class HexType { field, swamp, hillside, hills };
enum class UnitType { infrantry, cavalary, dragoons, artillery };

/// Components

using Movability = int;

struct MovementComponent : public ComponentBase {
    constexpr MovementComponent() = default;
    explicit MovementComponent(Movability moving_points);

    Movability moving_pts{};
    bool immobilized{false};

    constexpr Movability total_moving_pts() const noexcept { return _total_moving_pts; }

    template <class Archive>
    void serialize(Archive& archive);

   private:
    Movability _total_moving_pts{};
};

template <class Archive>
void MovementComponent::serialize(Archive& archive) {
    archive(cereal::base_class<ComponentBase>(this), CEREAL_NVP(_total_moving_pts),
            CEREAL_NVP(moving_pts), CEREAL_NVP(immobilized));
}

using Strength = int;
struct DirectFightComponent : public ComponentBase {
    constexpr DirectFightComponent() = default;
    constexpr DirectFightComponent(Strength strength_points) noexcept
        : strength_pts{strength_points} {}

    Strength strength_pts{};
    bool in_fight{false};

    template <class Archive>
    void serialize(Archive& archive);
};

template <class Archive>
void DirectFightComponent::serialize(Archive& archive) {
    archive(cereal::base_class<ComponentBase>(this), CEREAL_NVP(strength_pts),
            CEREAL_NVP(in_fight));
}

// Systems
class MovementSystem {
   public:
    explicit MovementSystem(const std::shared_ptr<Scenario>& scenario) noexcept;

    bool init_movement(HexCoordinate coord, std::vector<std::string> teams,
                       std::vector<std::string> hostile_teams);

    bool is_moving() const noexcept;
    void reset() noexcept;
    std::vector<Map::SiteId> path_indices(HexCoordinate destination) const;
    std::vector<HexCoordinate> path_preview(HexCoordinate destination) const;

    std::vector<std::unique_ptr<Action>> move_target(HexCoordinate destination);

   private:
    const MovementComponent* _target_mc{nullptr};
    const PositionComponent* _target_pc{nullptr};

    const std::shared_ptr<Scenario> _scenario{nullptr};

    std::set<Map::SiteId> _sticky_sites{};
    std::map<Map::SiteId, Movability> _distances{};
    std::map<Map::SiteId, Map::SiteId> _paths{};
};

/// System
class SystemKircholm : public SystemState {
   public:
    SystemKircholm();
    virtual ~SystemKircholm() = default;

    virtual void start() override;
    virtual void next_phase() override;
    virtual void handle_hex_event(const HexCoordinate& hex,
                                  SystemState::HexEvent ev) override;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    enum class StatePhase { not_started, movement, bombardment, attack, counterattack };
    StatePhase _current_phase{StatePhase::not_started};

    MovementSystem _movement;
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