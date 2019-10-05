#include "types.h"

#include <algorithm>

#include "core/log.h"

HexSite::HexSite(HexCoordinate coord, HexType type) noexcept
    : _coord{coord}, _type{type} {}

const HexCoordinate& HexSite::coord() const noexcept { return _coord; }

const HexType& HexSite::type() const noexcept { return _type; }

RiverSite::RiverSite() noexcept : _side1{0, 0}, _side2{1, 0}, _type{RiverType::small} {}

RiverSite::RiverSite(HexCoordinate side1, HexCoordinate side2, RiverType type)
    : _side1{side1}, _side2{side2}, _type{type} {
    const auto neighbors = side1.neighbors();
    engine_assert_throw(std::any_of(neighbors.cbegin(), neighbors.cend(),
                                    [&side2](const auto& n) { return n == side2; }),
                        "Creating RiverSite with non neighboring sides {} and {}.", side1,
                        side2);
}

std::pair<HexCoordinate, HexCoordinate> RiverSite::sides() const noexcept {
    return std::make_pair(_side1, _side2);
}

const RiverType& RiverSite::type() const noexcept { return _type; }