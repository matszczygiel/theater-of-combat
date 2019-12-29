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

class PathSearcher {
   public:
    void reset(const WeightedUnidirectionalGraph<std::pair<Map::SiteId, int>, Movability>&
                   graph);

    void search_path(Map::SiteId start, int start_dir, Movability max_cost);
    void search_rotation(Map::SiteId site, int start_dir, Movability max_cost);

    std::vector<std::tuple<Map::SiteId, int, Movability>> path_indices(
        Map::SiteId destination, int direction) const;

    const std::map<std::pair<Map::SiteId, int>, Movability>& distances() const;
    const std::map<std::pair<Map::SiteId, int>, std::pair<Map::SiteId, int>>& paths()
        const;

   private:
    WeightedUnidirectionalGraph<std::pair<Map::SiteId, int>, Movability> _graph{};
    std::map<std::pair<Map::SiteId, int>, Movability> _distances{};
    std::map<std::pair<Map::SiteId, int>, std::pair<Map::SiteId, int>> _paths{};
};
class MovementSystem {
   public:
    MovementSystem(const std::shared_ptr<Scenario>& scenario,
                            SystemState* system) noexcept;

    bool init_movement(HexCoordinate coord, std::vector<std::string> teams,
                       std::vector<std::string> hostile_teams);

    bool is_moving() const noexcept;
    bool is_hex_set() const noexcept;

    bool set_target_hex(HexCoordinate hex);
    std::map<int, Movability> get_avaliable_dirs();
    bool set_target_dir(int dir);
    bool move();

    void reset() noexcept;
    std::vector<std::tuple<HexCoordinate, int, Movability>> path_preview(
        HexCoordinate destination) const;

    const std::vector<std::tuple<HexCoordinate, int, Movability>>& path() const;

    WeightedUnidirectionalGraph<std::pair<Map::SiteId, int>, Movability>
    make_weighted_graph(Unit::IdType id) const;

   private:
    std::vector<std::tuple<HexCoordinate, int, Movability>> _path{};
    bool _immobilized{false};

    const MovementComponent* _target_mc{nullptr};
    const PositionComponent* _target_pc{nullptr};

    const std::shared_ptr<Scenario> _scenario{nullptr};

    std::vector<HexCoordinate> _sticky_sites{};
    PathSearcher _searcher{};

    SystemState* _system{nullptr};
};

}  // namespace kirch

#endif