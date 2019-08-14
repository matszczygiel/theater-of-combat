#include "types.h"

#include <algorithm>

HexSite::HexSite(HexCoordinate coord, HexType type) noexcept
    : _coord{coord}, _type{type} {}

const HexCoordinate& HexSite::coord() const { return _coord; }

const HexType& HexSite::type() const { return _type; }

RiverSite::RiverSite(HexCoordinate side1, HexCoordinate side2, RiverType type)
    : _side1{side1}, _side2{side2}, _type{type} {
    const auto neighbors = side1.neighbors();
    if (!std::any_of(neighbors.begin(), neighbors.end(),
                     [&](const auto& x) { return x == side2; }))
        throw std::runtime_error(
            "Creating RiverSite with non neighboring sides.");
}

std::pair<const HexCoordinate&, const HexCoordinate&> RiverSite::sides() const {
    return std::make_pair(_side1, _side2);
}

const RiverType& RiverSite::type() const { return _type; }