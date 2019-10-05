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
    HexSite() = default;
    HexSite(HexCoordinate coord, HexType type) noexcept;

    const HexCoordinate& coord() const noexcept;
    const HexType& type() const noexcept;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    HexCoordinate _coord{};
    HexType _type{};
};

template <class Archive>
void HexSite::serialize(Archive& archive) {
    archive(CEREAL_NVP(_coord), CEREAL_NVP(_type));
}

class RiverSite {
   public:
    RiverSite() noexcept;
    RiverSite(HexCoordinate side1, HexCoordinate side2, RiverType type);

    std::pair<HexCoordinate, HexCoordinate> sides() const noexcept;
    const RiverType& type() const noexcept;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    HexCoordinate _side1{};
    HexCoordinate _side2{};
    RiverType _type{};
};

template <class Archive>
void RiverSite::serialize(Archive& archive) {
    archive(CEREAL_NVP(_side1), CEREAL_NVP(_side2), CEREAL_NVP(_type));
}

#endif
