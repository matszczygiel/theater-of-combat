#ifndef TYPES_H
#define TYPES_H

#include <utility>

#include <cereal/types/utility.hpp>

#include "hexagons.h"

class HexSite {
   public:
    using HexType = int;

    constexpr HexSite() = default;
    constexpr explicit HexSite(HexCoordinate coord, HexType type) noexcept
        : _coord{coord}, _type{type} {}

    constexpr const HexCoordinate& coord() const noexcept { return _coord; };
    constexpr HexType type() const noexcept { return _type; };

    template <class Archive>
    void serialize(Archive& archive);

   private:
    HexCoordinate _coord{};
    HexType _type{0};
};

template <class Archive>
void HexSite::serialize(Archive& archive) {
    archive(CEREAL_NVP(_coord), CEREAL_NVP(_type));
}

class BorderSite {
   public:
    using BorderType = int;

    constexpr BorderSite() = default;
    explicit BorderSite(HexCoordinate side1, HexCoordinate side2, BorderType type);

    constexpr std::pair<HexCoordinate, HexCoordinate> sides() const noexcept;
    constexpr BorderType type() const noexcept { return _type; }

    template <class Archive>
    void serialize(Archive& archive);

   private:
    HexCoordinate _side1{0, 0};
    HexCoordinate _side2{1, 0};
    BorderType _type{0};
};

constexpr std::pair<HexCoordinate, HexCoordinate> BorderSite::sides() const noexcept {
    return std::make_pair(_side1, _side2);
}

template <class Archive>
void BorderSite::serialize(Archive& archive) {
    archive(CEREAL_NVP(_side1), CEREAL_NVP(_side2), CEREAL_NVP(_type));
}

#endif
