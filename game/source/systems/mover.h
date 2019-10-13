#ifndef MOVER_H
#define MOVER_H

#include <memory>

#include "gameplay/action.h"
#include "gameplay/game_state.h"
#include "toc/map/graph.h"
#include "toc/map/map.h"
#include "toc/unit/unit_manager.h"

namespace mover {

class MovementSystem {
   public:
    explicit MovementSystem(const GameState& state) noexcept;

    bool init_movement(HexCoordinate coord, std::vector<std::string> teams,
                       std::vector<std::string> hostile_teams);

    bool is_moving() const noexcept;
    void reset() noexcept;
    std::vector<Map::SiteId> path_indices(HexCoordinate destination) const;
    std::vector<HexCoordinate> path_preview(HexCoordinate destination) const;

    std::unique_ptr<ComponentChangeAction<MovementComponent>> move_target(
        HexCoordinate destination);

   private:
    const MovementComponent* _target_component{nullptr};

    const std::shared_ptr<Scenario> _scenario{nullptr};

    std::set<Map::SiteId> _sticky_sites{};
    std::map<Map::SiteId, Movability> _distances{};
    std::map<Map::SiteId, Map::SiteId> _paths{};
};

}  // namespace mover

#endif