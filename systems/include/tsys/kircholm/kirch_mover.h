#ifndef KIRCH_MOVER_H
#define KIRCH_MOVER_H

#include <memory>
#include <vector>
#include <map>

#include "toc/map/hexagons.h"
#include "toc/gameplay/scenario.h"
#include "toc/gameplay/action.h"

#include "kirch_components.h"

namespace kirch {
class MovementSystem {
   public:
    explicit MovementSystem(const std::shared_ptr<Scenario>& scenario) noexcept;

    bool init_movement(HexCoordinate coord, std::vector<std::string> teams,
                       std::vector<std::string> hostile_teams);

    bool is_moving() const noexcept;
    void reset() noexcept;
    std::vector<Map::SiteId> path_indices(HexCoordinate destination) const;
    std::vector<HexCoordinate> path_preview(HexCoordinate destination) const;

    std::vector<std::unique_ptr<Action>> move_target(HexCoordinate destination);

   private:
    const MovementComponent* _target_mc{nullptr};
    const PositionComponent* _target_pc{nullptr};

    const std::shared_ptr<Scenario> _scenario{nullptr};

    std::set<Map::SiteId> _sticky_sites{};
    std::map<Map::SiteId, Movability> _distances{};
    std::map<Map::SiteId, Map::SiteId> _paths{};
};

}  // namespace kirch

#endif