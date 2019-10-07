#include "map.h"

#include <algorithm>
#include <tuple>
#include <vector>

#include <cereal/archives/json.hpp>

#include "core/log.h"

const std::map<Map::SiteId, HexSite>& Map::hexes() const noexcept { return _hexes; }

const std::map<Map::SiteId, RiverSite>& Map::rivers() const noexcept { return _rivers; }

const BidirectionalGraph<int>& Map::graph() const noexcept { return _graph; }

Map& Map::insert(HexSite site) {
    if (auto it = std::find_if(
            _hexes.begin(), _hexes.end(),
            [&site](const auto& x) { return x.second.coord() == site.coord(); });
        it != _hexes.end()) {
        it->second = site;
        return *this;
    }

    const auto neighors = site.coord().neighbors();

    std::set<Map::SiteId> found_neighbors{};
    for (const auto& [id, hex] : _hexes) {
        if (std::any_of(neighors.begin(), neighors.end(),
                        [hex = hex](const auto& x) { return (x == hex.coord()); })) {
            found_neighbors.insert(id);
        }
    }

    const auto id = _id_gen.fetch();
    engine_assert(_hexes.insert({id, site}).second, "");
    _graph.insert_node(id, found_neighbors);
    return *this;
}

Map& Map::insert(RiverSite site) {
    const auto [side1, side2] = site.sides();

    std::vector<Map::SiteId> found_hexes{};
    for (const auto& [s_id, s_site] : _hexes) {
        if (s_site.coord() == side1 || s_site.coord() == side2) {
            found_hexes.push_back(s_id);
        }
    }

    if (found_hexes.size() < 2) {
        engine_assert_throw(false,
                            "Map doesn't contain sides of the river site {} and {}.\n",
                            side1, side2);

    } else if (found_hexes.size() > 2) {
        engine_assert(false, "Found to many hexes, indicating deeper logic error");
    }

    if (auto it = std::find_if(_rivers.begin(), _rivers.end(),
                               [side1 = side1, side2 = side2](const auto& riv) {
                                   const auto [s1, s2] = riv.second.sides();
                                   if (s1 == side1 && s2 == side2) {
                                       return true;
                                   } else if (s1 == side2 && s2 == side1) {
                                       return true;
                                   }
                                   return false;
                               });
        it != _rivers.end()) {
        it->second = site;
        return *this;
    }

    const auto id = _id_gen.fetch();
    engine_assert(_rivers.insert({id, site}).second, "");
    _graph.insert_node(id, {found_hexes[0], found_hexes[1]})
        .remove_edge(found_hexes[0], found_hexes[1]);
    return *this;
}

Map::SiteType Map::type_of(Map::SiteId id) const {
    if (_hexes.count(id) == 1)
        return SiteType::hex;
    else if (_rivers.count(id) == 1)
        return SiteType::river;
    engine_assert(false, "Unknown site type.");
    return SiteType::hex;
}

Map Map::create_test_map() {
    Map map{};
    int offset_q = 6;
    for (int p = -10; p <= 10; ++p) {
        if (p % 2 == 0) {
            offset_q -= 1;
        }
        for (int q = -10; q <= 10; ++q) {
            if (q < 0 && p < 0) {
                map.insert(HexSite{
                    HexCoordinate(q + offset_q, p),
                    HexType::forest,
                });
            } else {
                map.insert(HexSite{
                    HexCoordinate(q + offset_q, p),
                    HexType::field,
                });
            }
        }
    }

    map.insert(RiverSite(HexCoordinate(-10, 7), HexCoordinate(-10, 8), RiverType::stream))
        .insert(RiverSite(HexCoordinate(-9, 7), HexCoordinate(-10, 8), RiverType::stream))
        .insert(RiverSite(HexCoordinate(-9, 7), HexCoordinate(-9, 8), RiverType::stream))
        .insert(RiverSite(HexCoordinate(-9, 7), HexCoordinate(-8, 7), RiverType::stream))
        .insert(RiverSite(HexCoordinate(-8, 6), HexCoordinate(-8, 7), RiverType::stream))
        .insert(RiverSite(HexCoordinate(-7, 6), HexCoordinate(-7, 7), RiverType::stream))
        .insert(RiverSite(HexCoordinate(-7, 6), HexCoordinate(-8, 7), RiverType::stream))
        .insert(RiverSite(HexCoordinate(-7, 7), HexCoordinate(-6, 6), RiverType::stream))
        .insert(RiverSite(HexCoordinate(-6, 7), HexCoordinate(-6, 6), RiverType::stream))
        .insert(RiverSite(HexCoordinate(-5, 6), HexCoordinate(-6, 7), RiverType::stream))
        .insert(RiverSite(HexCoordinate(-5, 7), HexCoordinate(-6, 7), RiverType::stream))
        .insert(RiverSite(HexCoordinate(0, -1), HexCoordinate(1, -1), RiverType::stream))
        .insert(RiverSite(HexCoordinate(0, 0), HexCoordinate(1, -1), RiverType::stream))
        .insert(RiverSite(HexCoordinate(0, 0), HexCoordinate(1, 0), RiverType::stream))
        .insert(RiverSite(HexCoordinate(1, 0), HexCoordinate(0, 1), RiverType::stream));

    return map;
}

std::optional<Map::SiteId> Map::get_hex_id(HexCoordinate coord) const noexcept {
    auto it = std::find_if(_hexes.begin(), _hexes.end(), [&](const auto& pair) {
        return pair.second.coord() == coord;
    });

    if (it != _hexes.end())
        return it->first;
    else
        return {};
}

std::optional<HexCoordinate> Map::get_hex_coord(SiteId id) const noexcept {
    auto it = _hexes.find(id);

    if (it != _hexes.end())
        return it->second.coord();
    else
        return {};
}

std::set<Map::SiteId> Map::get_controlable_hexes_from(SiteId id) const {
    const auto it = _graph.adjacency_matrix().find(id);
    if (it == _graph.adjacency_matrix().end())
        return {};

    auto neighbors = it->second;
    for (const auto& node : it->second) {
        switch (type_of(node)) {
            case SiteType::river: {
                auto riv_neighbors = _graph.adjacency_matrix().at(node);
                neighbors.merge(riv_neighbors);
            } break;
            default:
                break;
        }
    }

    neighbors.erase(id);

    for (auto i = neighbors.begin(); i != neighbors.end();) {
        if (type_of(*i) != SiteType::hex) {
            i = neighbors.erase(i);
        } else {
            ++i;
        }
    }
    return neighbors;
}
