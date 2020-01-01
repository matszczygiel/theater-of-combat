#ifndef UNIT_COMPONENTS_H
#define UNIT_COMPONENTS_H

#include <optional>

#include <cereal/types/base_class.hpp>

#include "cereal/optional.hpp"

#include "toc/map/hexagons.h"
#include "unit.h"

struct PositionComponent : public ComponentBase {
    constexpr PositionComponent() = default;
    constexpr explicit PositionComponent(const HexCoordinate& coord) : position{coord} {}
    constexpr PositionComponent(const HexCoordinate& coord, int dir)
        : position{coord}, direction{dir % HexCoordinate::neighbors_count()} {}

    std::optional<HexCoordinate> position{};
    std::optional<int> direction{};

    template <class Archive>
    void serialize(Archive& archive);
};

template <class Archive>
void PositionComponent::serialize(Archive& archive) {
    archive(cereal::base_class<ComponentBase>(this), CEREAL_NVP(position),
            CEREAL_NVP(direction));
}

#endif