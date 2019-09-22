#ifndef MOVER_H
#define MOVER_H

#include <memory>

#include "gameplay/action.h"
#include "gameplay/game_state.h"
#include "map/graph.h"
#include "map/map.h"
#include "unit/unit_manager.h"

struct MovementComponent;

namespace mover {
WeightedBidirectionalGraph make_weighted_graph(const Map& map, UnitType type);

class MovementSystem {
   public:
    explicit MovementSystem(const GameState& state);

    bool init_movement(HexCoordinate coord, std::vector<std::string> teams,
                       std::vector<std::string> hostile_teams);

    bool is_moving() const;
    void reset();
    std::vector<int> path_indices(HexCoordinate destination) const;
    std::vector<HexCoordinate> path_preview(HexCoordinate destination) const;

    std::unique_ptr<MovementAction> move_target(HexCoordinate destination);

   private:
    const MovementComponent* _target_component{nullptr};

    const std::shared_ptr<UnitManager> _units{nullptr};
    const std::shared_ptr<Map> _map{nullptr};

    std::map<std::string, std::set<Unit::IdType>> _teams{};

    std::set<int> _sticky_sites{};
    std::map<int, int> _distances{};
    std::map<int, int> _paths{};
};

}  // namespace mover

#endif