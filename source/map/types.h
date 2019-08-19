#ifndef TYPES_H
#define TYPES_H

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
    HexSite(HexCoordinate coord, HexType type) noexcept;

    const HexCoordinate& coord() const;
    const HexType& type() const;

   private:
    HexCoordinate _coord{};
    HexType _type{};
};

class RiverSite {
   public:
    RiverSite(HexCoordinate side1, HexCoordinate side2, RiverType type);

    std::pair<HexCoordinate, HexCoordinate> sides() const;
    const RiverType& type() const;

   private:
    HexCoordinate _side1{};
    HexCoordinate _side2{};
    RiverType _type{};
};

#endif