#ifndef KIRCHOLM_H
#define KIRCHOLM_H

#include <optional>

#include <cereal/types/base_class.hpp>

#include "cereal/optional.hpp"

#include "toc/gameplay/scenario.h"
#include "toc/map/hexagons.h"
#include "toc/unit/unit_components.h"

namespace kirch {

///Basic types
enum class HexType { field, swamp, hillside, hills };
enum class UnitType { infrantry, cavalary, dragoons, artillery };

///Components

using Movability = int;

struct MovementComponent : public ComponentBase {
    constexpr MovementComponent() = default;
    explicit MovementComponent(Movability moving_points);

    Movability moving_pts{};
    std::optional<HexCoordinate> position{};
    int direction{};
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
            CEREAL_NVP(moving_pts), CEREAL_NVP(position), CEREAL_NVP(direction),
            CEREAL_NVP(immobilized));
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

//Systems

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

}  // namespace kirch

#endif