#ifndef UNIT_COMPONENTS_H
#define UNIT_COMPONENTS_H

#include <optional>

#include <cereal/types/base_class.hpp>

#include "cereal/optional.hpp"

#include "toc/map/hexagons.h"
#include "unit.h"

using Movability = int;
struct MovementComponent : public ComponentBase {
    constexpr MovementComponent() = default;
    explicit MovementComponent(Movability moving_points);

    Movability moving_pts{};
    std::optional<HexCoordinate> position{};
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
            CEREAL_NVP(moving_pts), CEREAL_NVP(position), CEREAL_NVP(immobilized));
}

using Strength = int;
struct FightComponent : public ComponentBase {
    constexpr FightComponent() = default;
    constexpr FightComponent(Strength strength_points) noexcept
        : strength_pts{strength_points} {}

    Strength strength_pts{};
    bool in_fight{false};

    template <class Archive>
    void serialize(Archive& archive);
};

template <class Archive>
void FightComponent::serialize(Archive& archive) {
    archive(cereal::base_class<ComponentBase>(this), CEREAL_NVP(strength_pts),
            CEREAL_NVP(in_fight));
}

struct PositionComponent : public ComponentBase {
    constexpr PositionComponent() = default;
    std::optional<HexCoordinate> position{};
    int direction{0};

    template <class Archive>
    void serialize(Archive& archive);
};

template <class Archive>
void PositionComponent::serialize(Archive& archive) {
    archive(cereal::base_class<ComponentBase>(this), CEREAL_NVP(position),
            CEREAL_NVP(direction));
}

#endif