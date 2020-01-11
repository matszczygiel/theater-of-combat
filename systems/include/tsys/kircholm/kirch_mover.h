#ifndef KIRCH_MOVER_H
#define KIRCH_MOVER_H

#include <map>
#include <memory>
#include <vector>

#include "toc/gameplay/scenario.h"
#include "toc/map/hexagons.h"

#include "kirch_component_system.h"
#include "kirch_components.h"

namespace kirch {

class PathSearcher {
   public:
    PathSearcher() = default;
    explicit PathSearcher(
        const WeightedUnidirectionalGraph<Map::SiteTypeId, Movability>& graph);

    void search_path(Map::SiteTypeId start, Movability max_cost);

    std::vector<std::pair<Map::SiteTypeId, Movability>> path_indices(
        Map::SiteTypeId destination) const;

    const std::map<Map::SiteTypeId, Movability>& distances() const;
    const std::map<Map::SiteTypeId, Map::SiteTypeId>& paths() const;

   private:
    WeightedUnidirectionalGraph<Map::SiteTypeId, Movability> _graph{};
    std::map<Map::SiteTypeId, Movability> _distances{};
    std::map<Map::SiteTypeId, Map::SiteTypeId> _paths{};
};

class MovementSystem : public ComponentSystemKircholm {
   public:
    enum class State { idle, paths_calculated, target_hex_set, target_dir_set };

    explicit MovementSystem(SystemKircholm* system) noexcept;

    void on_left_click(HexCoordinate coord);
    void on_right_click(HexCoordinate coord);
    void on_over(HexCoordinate coord);
    void on_left_realease(HexCoordinate coord);
    void on_right_realease(HexCoordinate coord);

    void update();

    State init_movement(HexCoordinate coord);
    State set_target_hex(HexCoordinate hex);
    State set_target_dir(int dir);
    State set_target_dir(HexCoordinate hex);
    State move();

    void reset_moving_pts();

    std::map<int, Movability> get_avaliable_dirs();
    std::vector<std::tuple<HexCoordinate, int, Movability>> make_path_preview(
        HexCoordinate destination, int dir) const;

    WeightedUnidirectionalGraph<Map::SiteTypeId, Movability> make_weighted_graph(
        Unit::IdType id);

    bool unit_can_enter_hostile_zone(Unit::IdType unit) const;

   private:
    std::vector<std::tuple<HexCoordinate, int, Movability>> _path{};

    const MovementComponent* _target_mc{nullptr};
    const PositionComponent* _target_pc{nullptr};

    bool _immobilized{false};
    HexCoordinate _target_hex{};
    int _target_direction{};
    std::map<HexCoordinate, std::set<int>> _hostile_zone{};
    PathSearcher _searcher{};

    State _state{State::idle};
};

}  // namespace kirch

#endif