#include "map.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>
#include <sstream>
#include <tuple>
#include <vector>

#include <cereal/archives/json.hpp>

#include "core/log.h"

const std::map<Map::SiteId, HexSite>& Map::hexes() const { return _hexes; }

const std::map<Map::SiteId, RiverSite>& Map::rivers() const { return _rivers; }

const BidirectionalGraph& Map::graph() const { return _graph; }

Map::SiteId Map::fetch_id() { return _current_free_id++; }

void Map::insert(HexSite site) {
    if (std::any_of(_hexes.begin(), _hexes.end(), [&](const auto& x) {
            return x.second.coord() == site.coord();
        }))
        throw std::invalid_argument("Map already contains such hex.");

    const auto neighors = site.coord().neighbors();

    std::set<Map::SiteId> found_neighbors;

    for (const auto& hex : _hexes) {
        if (std::any_of(neighors.begin(), neighors.end(), [&](const auto& x) {
                return (x == hex.second.coord());
            })) {
            engine_assert(found_neighbors.insert(hex.first).second, "");
        }
    }

    const auto id = fetch_id();
    engine_assert(_hexes.insert({id, site}).second, "");
    _graph.insert_node(id, found_neighbors);
}

void Map::insert(RiverSite site) {
    std::vector<Map::SiteId> found_hexes;

    for (const auto& [s_id, s_site] : _hexes) {
        const auto [side1, side2] = site.sides();
        if (s_site.coord() == side1 || s_site.coord() == side2) {
            found_hexes.push_back(s_id);
        }
    }

    if (found_hexes.size() < 2) {
        std::stringstream ss;
        ss << "Map doesn't contain sides of the river site.\n"
           << "Found hexes id:";
        for (auto id : found_hexes)
            ss << "  " << id;
        ss << '.';

        throw std::logic_error(ss.str());

    } else if (found_hexes.size() > 2) {
        engine_assert(false,
                      "Found to many hexes, indicating deeper logic error");
    }

    if (std::any_of(_rivers.begin(), _rivers.end(), [&](const auto& riv) {
            const auto [side1, side2] = site.sides();
            if (riv.second.sides().first == side1 &&
                riv.second.sides().second == side2) {
                return true;
            } else if (riv.second.sides().first == side2 &&
                       riv.second.sides().second == side1) {
                return true;
            }
            return false;
        })) {
        throw std::logic_error("Already found such river.");
    }

    const auto id = fetch_id();
    engine_assert(_rivers.insert({id, site}).second, "");
    _graph.insert_node(id, {found_hexes[0], found_hexes[1]});
    _graph.remove_edge(found_hexes[0], found_hexes[1]);
}

Map::SiteType Map::type_of(Map::SiteId id) const {
    if(_hexes.count(id) == 1)
        return SiteType::hex;
    else if (_rivers.count(id) == 1)
        return SiteType::river;
    app_assert(false, "Unknown site type.");
    return SiteType::hex;
}

Map Map::create_test_map() {
    Map map;
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

    map.insert(RiverSite(HexCoordinate(-10, 7), HexCoordinate(-10, 8),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(-9, 7), HexCoordinate(-10, 8),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(-9, 7), HexCoordinate(-9, 8),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(-9, 7), HexCoordinate(-8, 7),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(-8, 6), HexCoordinate(-8, 7),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(-7, 6), HexCoordinate(-7, 7),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(-7, 6), HexCoordinate(-8, 7),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(-7, 7), HexCoordinate(-6, 6),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(-6, 7), HexCoordinate(-6, 6),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(-5, 6), HexCoordinate(-6, 7),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(-5, 7), HexCoordinate(-6, 7),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(0, -1), HexCoordinate(1, -1),
                         RiverType::stream));
    map.insert(RiverSite(HexCoordinate(0, 0), HexCoordinate(1, -1),
                         RiverType::stream));
    map.insert(
        RiverSite(HexCoordinate(0, 0), HexCoordinate(1, 0), RiverType::stream));
    map.insert(
        RiverSite(HexCoordinate(1, 0), HexCoordinate(0, 1), RiverType::stream));

    return map;
}