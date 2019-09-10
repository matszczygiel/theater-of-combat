#ifndef MOVER_H
#define MOVER_H

#include "map/graph.h"
#include "map/map.h"
#include "unit/unit.h"

namespace mover {
WeightedBidirectionalGraph make_weighted_graph(const Map& map, UnitType type);
}

#endif