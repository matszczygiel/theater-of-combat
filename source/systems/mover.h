#ifndef MOVER_H
#define MOVER_H

#include <memory>

#include "map/graph.h"
#include "map/map.h"
#include "unit/unit_manager.h"

struct MovementComponent;

namespace mover {
WeightedBidirectionalGraph make_weighted_graph(const Map& map, UnitType type);

class MovementSystem {
   public:
    explicit MovementSystem(std::shared_ptr<UnitManager>& units,
                            const std::shared_ptr<Map>& map);

    bool init_movement(HexCoordinate coord);

    bool is_moving() const;
    void reset();
    std::vector<int> path_indices(HexCoordinate destination) const;
    std::vector<HexCoordinate> path_preview(HexCoordinate destination) const;

    bool move_target(HexCoordinate destination);

   private:
    MovementComponent* _target_component{nullptr};

    std::shared_ptr<UnitManager> _units{nullptr};
    std::shared_ptr<Map> _map{nullptr};

    std::map<int, int> _distances{};
    std::map<int, int> _paths{};
};

}  // namespace mover

#endif