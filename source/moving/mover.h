#ifndef MOVER_H
#define MOVER_H

#include <memory>

#include "map/graph.h"
#include "map/map.h"
#include "unit/unit.h"

namespace mover {
WeightedBidirectionalGraph make_weighted_graph(const Map& map, UnitType type);

class MovementSystem {
   public:
    explicit MovementSystem(std::shared_ptr<UnitManager>& units,
                            const std::shared_ptr<Map>& map);

    bool init_movement(HexCoordinate coord);

   private:
    std::shared_ptr<UnitManager> _units;
    std::shared_ptr<Map> _map;

    std::map<int, int> _distances;
    std::map<int, int> _paths;
};

}  // namespace mover

#endif