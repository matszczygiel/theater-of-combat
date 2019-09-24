#ifndef UNIT_COMPONENTS_H
#define UNIT_COMPONENTS_H

#include <optional>

#include <cereal/types/base_class.hpp>

#include "cereal/optional.hpp"

#include "map/hexagons.h"
#include "unit.h"

struct MovementComponent : public ComponentBase {
    MovementComponent() = default;
    explicit MovementComponent(int moving_points) noexcept;

    int moving_pts{};
    std::optional<HexCoordinate> position{};
    bool immobilized{false};

    const int& total_moving_pts() const;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    int _total_moving_pts{};
};

template <class Archive>
void MovementComponent::serialize(Archive& archive) {
    archive(cereal::base_class<ComponentBase>(this), CEREAL_NVP(_total_moving_pts),
            CEREAL_NVP(moving_pts), CEREAL_NVP(position), CEREAL_NVP(immobilized));
}

#endif