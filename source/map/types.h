#ifndef TYPES_H
#define TYPES_H

#include <cereal/types/utility.hpp>

#include "hexagons.h"

enum class HexType {
    field,
    forest,
};

enum class RiverType {
    small,
    stream,
};

class HexSite {
   public:
    constexpr HexSite() = default;
    constexpr explicit HexSite(HexCoordinate coord, HexType type) noexcept
        : _coord{coord}, _type{type} {}

    constexpr const HexCoordinate& coord() const noexcept { return _coord; };
    constexpr HexType type() const noexcept { return _type; };

    template <class Archive>
    void serialize(Archive& archive);

   private:
    HexCoordinate _coord{};
    HexType _type{HexType::field};
};

template <class Archive>
void HexSite::serialize(Archive& archive) {
    archive(CEREAL_NVP(_coord), CEREAL_NVP(_type));
}

class RiverSite {
   public:
    constexpr RiverSite() noexcept
        : _side1{0, 0}, _side2{1, 0}, _type{RiverType::small} {}
    explicit RiverSite(HexCoordinate side1, HexCoordinate side2, RiverType type);

    constexpr std::pair<HexCoordinate, HexCoordinate> sides() const noexcept;
    constexpr RiverType type() const noexcept { return _type; }

    template <class Archive>
    void serialize(Archive& archive);

   private:
    HexCoordinate _side1{};
    HexCoordinate _side2{};
    RiverType _type{};
};

constexpr std::pair<HexCoordinate, HexCoordinate> RiverSite::sides() const noexcept {
    return std::make_pair(_side1, _side2);
}

template <class Archive>
void RiverSite::serialize(Archive& archive) {
    archive(CEREAL_NVP(_side1), CEREAL_NVP(_side2), CEREAL_NVP(_type));
}

#endif
