#include "map/map.h"

#include <algorithm>
#include <tuple>
#include <vector>

#include <cereal/archives/json.hpp>

#include "core/log.h"

const std::map<Map::SiteId, HexSite>& Map::hexes() const noexcept { return _hexes; }

const std::map<Map::SiteId, BorderSite>& Map::borders() const noexcept {
    return _borders;
}

const BidirectionalGraph<std::pair<Map::SiteId, int>>& Map::graph() const noexcept {
    return _graph;
}

Map& Map::insert(HexSite site) {
    if (auto it = std::find_if(
            _hexes.begin(), _hexes.end(),
            [&site](const auto& x) { return x.second.coord() == site.coord(); });
        it != _hexes.end()) {
        it->second = site;
        return *this;
    }

    const auto id = _id_gen.fetch();
    engine_assert(_hexes.insert({id, site}).second, "");

    const auto neighors  = site.coord().neighbors();
    constexpr int n_size = neighors.size();

    for (int i = 0; i < n_size; ++i) {
        if (const auto it = std::find_if(
                _hexes.cbegin(), _hexes.cend(),
                [&neighors, &i](const auto& x) { return x.second.coord() == neighors[i]; });
            it != _hexes.cend()) {
            const auto n_direction = (i + (n_size / 2)) % n_size;
            _graph.insert_node({id, i}, {{it->first, n_direction}});
        }
    }

    for (int i = 0; i < n_size; ++i) {
        _graph.insert_edge({id, i}, {id, (i + 1) % n_size});
    }

    return *this;
}

Map& Map::insert(BorderSite site) {
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

    if (auto it = std::find_if(_borders.begin(), _borders.end(),
                               [side1 = side1, side2 = side2](const auto& riv) {
                                   const auto [s1, s2] = riv.second.sides();
                                   if (s1 == side1 && s2 == side2) {
                                       return true;
                                   } else if (s1 == side2 && s2 == side1) {
                                       return true;
                                   }
                                   return false;
                               });
        it != _borders.end()) {
        it->second = site;
        return *this;
    }

    const auto id = _id_gen.fetch();
    engine_assert(_borders.insert({id, site}).second, "");

    const auto hex_0         = *get_hex_coord(found_hexes[0]);
    const auto hex_1         = *get_hex_coord(found_hexes[1]);
    const auto neigbors_of_0 = hex_0.neighbors();
    const auto it = std::find(neigbors_of_0.cbegin(), neigbors_of_0.cend(), hex_1);
    engine_assert(it != neigbors_of_0.cend(), "");
    const int dir_from_0 = std::distance(neigbors_of_0.cbegin(), it);
    const int dir_from_1 = (dir_from_0 + neigbors_of_0.size() / 2) % neigbors_of_0.size();

    _graph.insert_node({id, dir_from_1}, {{found_hexes[0], dir_from_0}})
        .insert_node({id, dir_from_0}, {{found_hexes[1], dir_from_1}})
        .remove_edge({found_hexes[0], dir_from_0}, {found_hexes[1], dir_from_1});

    return *this;
}

Map::SiteType Map::type_of(Map::SiteId id) const {
    if (_hexes.count(id) == 1)
        return SiteType::hex;
    else if (_borders.count(id) == 1)
        return SiteType::border;
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
                map.insert(HexSite{HexCoordinate(q + offset_q, p), 1});
            } else {
                map.insert(HexSite{HexCoordinate(q + offset_q, p), 0});
            }
        }
    }
/*
    map.insert(BorderSite(HexCoordinate(-10, 7), HexCoordinate(-10, 8), 0))
        .insert(BorderSite(HexCoordinate(-9, 7), HexCoordinate(-10, 8), 0))
        .insert(BorderSite(HexCoordinate(-9, 7), HexCoordinate(-9, 8), 0))
        .insert(BorderSite(HexCoordinate(-9, 7), HexCoordinate(-8, 7), 0))
        .insert(BorderSite(HexCoordinate(-8, 6), HexCoordinate(-8, 7), 0))
        .insert(BorderSite(HexCoordinate(-7, 6), HexCoordinate(-7, 7), 0))
        .insert(BorderSite(HexCoordinate(-7, 6), HexCoordinate(-8, 7), 0))
        .insert(BorderSite(HexCoordinate(-7, 7), HexCoordinate(-6, 6), 0))
        .insert(BorderSite(HexCoordinate(-6, 7), HexCoordinate(-6, 6), 0))
        .insert(BorderSite(HexCoordinate(-5, 6), HexCoordinate(-6, 7), 0))
        .insert(BorderSite(HexCoordinate(-5, 7), HexCoordinate(-6, 7), 0))
        .insert(BorderSite(HexCoordinate(0, -1), HexCoordinate(1, -1), 0))
        .insert(BorderSite(HexCoordinate(0, 0), HexCoordinate(1, -1), 0))
        .insert(BorderSite(HexCoordinate(0, 0), HexCoordinate(1, 0), 0))
        .insert(BorderSite(HexCoordinate(1, 0), HexCoordinate(0, 1), 0));
*/
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
    const auto hex = get_hex_coord(id);
    if (!hex)
        return {};

    std::set<Map::SiteId> res;
    for (const auto coord : hex->neighbors()) {
        const auto n_id = get_hex_id(coord);
        if (n_id)
            res.insert(*n_id);
    }
    return res;
}
