#include "map/types.h"

#include <algorithm>

#include "core/log.h"

RiverSite::RiverSite(HexCoordinate side1, HexCoordinate side2, RiverType type)
    : _side1{side1}, _side2{side2}, _type{type} {
    const auto neighbors = side1.neighbors();
    engine_assert_throw(std::any_of(neighbors.cbegin(), neighbors.cend(),
                                    [&side2](const auto& n) { return n == side2; }),
                        "Creating RiverSite with non neighboring sides {} and {}.", side1,
                        side2);
}
