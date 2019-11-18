#ifndef KIRCH_MOVER_H
#define KIRCH_MOVER_H

#include <map>
#include <memory>
#include <vector>

#include "toc/gameplay/action.h"
#include "toc/gameplay/scenario.h"
#include "toc/map/hexagons.h"

#include "kirch_components.h"

namespace kirch {
class MovementSystem {
   public:
    explicit MovementSystem(const std::shared_ptr<Scenario>& scenario) noexcept;

    bool init_movement(HexCoordinate coord, std::vector<std::string> teams,
                       std::vector<std::string> hostile_teams);

    bool is_moving() const noexcept;
    void reset() noexcept;
    std::vector<std::tuple<Map::SiteId, int, Movability>> path_indices(
        HexCoordinate destination, int direction) const;
    std::vector<std::tuple<HexCoordinate, int, Movability>> path_preview(
        HexCoordinate destination, int direction) const;

    std::vector<std::unique_ptr<Action>> move_target(HexCoordinate destination,
                                                     int direction);

    WeightedUnidirectionalGraph<std::pair<Map::SiteId, int>, Movability>
    make_weighted_graph(Unit::IdType id) const;

   private:
    const MovementComponent* _target_mc{nullptr};
    const PositionComponent* _target_pc{nullptr};

    const std::shared_ptr<Scenario> _scenario{nullptr};

    std::set<Map::SiteId> _sticky_sites{};
    std::map<std::pair<Map::SiteId, int>, Movability> _distances{};
    std::map<std::pair<Map::SiteId, int>, std::pair<Map::SiteId, int>> _paths{};
};

}  // namespace kirch

#endif