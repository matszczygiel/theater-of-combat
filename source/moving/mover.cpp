#include "mover.h"

#include "unit/unit_components.h"

namespace mover {
static std::map<HexType, int> get_hex_table(UnitType type) {
    switch (type) {
        case UnitType::heavy:
            return {
                {HexType::field, 2},
                {HexType::forest, 6},
            };

        case UnitType::mechanized:
            return {
                {HexType::field, 2},
                {HexType::forest, 5},
            };

        default:
            app_assert(false, "Unknown unit type.");
            return {};
    }
}

static std::map<RiverType, int> get_river_table(UnitType type) {
    switch (type) {
        case UnitType::heavy:
            return {
                {RiverType::small, 3},
                {RiverType::stream, 4},
            };

        case UnitType::mechanized:
            return {
                {RiverType::small, 3},
                {RiverType::stream, 3},
            };

        default:
            app_assert(false, "Unknown unit type.");
            return {};
    }
}

WeightedBidirectionalGraph make_weighted_graph(const Map& map, UnitType type) {
    const auto river_tab = get_river_table(type);
    const auto hex_tab   = get_hex_table(type);

    WeightedBidirectionalGraph res(map.graph(), 0);

    for (const auto& [node, neighbors] : map.graph().adjacency_matrix()) {
        const auto site_type = map.type_of(node);

        switch (site_type) {
            case Map::SiteType::hex: {
                const auto hex_type = map.hexes().at(node).type();

                for (const auto& neighbor : neighbors) {
                    const auto w = res.weight(neighbor, node);
                    res.change_edge_weight(neighbor, node,
                                           w + hex_tab.at(hex_type));
                }
            } break;
            case Map::SiteType::river: {
                const auto riv_type = map.rivers().at(node).type();
                app_assert(neighbors.size() == 2,
                           "River should have 2 neighbors.");
                const auto hex1 = *neighbors.begin();
                const auto hex2 = *(++neighbors.begin());

                const auto w12 = res.weight(hex1, node) +
                                 res.weight(node, hex2) +
                                 river_tab.at(riv_type);

                const auto w21 = res.weight(hex2, node) +
                                 res.weight(node, hex1) +
                                 river_tab.at(riv_type);

                res.remove_node(node);
                res.insert_edge(hex1, hex2, w12, w21);
            } break;

            default:
                app_assert(false,
                           "Unknown site type in computing weighted graph.");
                break;
        }
    }
    return res;
}

MovementSystem::MovementSystem(std::shared_ptr<UnitManager>& units,
                               const std::shared_ptr<Map>& map)
    : _units{units}, _map{map} {}

bool MovementSystem::init_movement(HexCoordinate coord) {
    MovementComponent* target_component{nullptr};
    _units->apply_for_each<MovementComponent>([&](auto& cmp) {
        if (cmp.position == coord) {
            target_component = std::addressof(cmp);
            return false;
        } else
            return true;
    });
    if (target_component == nullptr)
        return false;

    const auto graph =
        make_weighted_graph(*_map, target_component->owner_type());

    Map::SiteId start_hex;
    for (const auto& [id, hex] : _map->hexes()) {
        if (hex.coord() == coord) {
            start_hex = id;
            break;
        }
    }

    auto [distances, paths] = graph.dijkstra(start_hex);
    for (auto i = distances.begin(); i != distances.end();) {
        if (i->second > target_component->moving_pts) {
            app_assert(
                paths.erase(i->first) == 1,
                "Paths does not containg such node, error in dijkstra algorithm ?");
            i = distances.erase(i);
        } else {
            ++i;
        }
    }

    _distances = std::move(distances);
    _paths     = std::move(paths);
}

}  // namespace mover