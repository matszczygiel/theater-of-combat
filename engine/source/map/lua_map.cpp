#include "map/lua_map.h"

#include <sstream>

#include <cereal/archives/json.hpp>
#include <sol/sol.hpp>

#include "map/hexagons.h"
#include "map/map.h"
#include "map/types.h"

namespace map {

static void push_coordinates(sol::state& lua) {
    auto coord = lua.new_usertype<HexCoordinate>(
        "Coord", sol::constructors<HexCoordinate(), HexCoordinate(int, int)>());
    coord["q"] = &HexCoordinate::q;
    coord["p"] = &HexCoordinate::p;
}

static void push_map(sol::state& lua) {
    auto map = lua.new_usertype<Map>("Map", sol::constructors<Map(), Map(const Map&)>());
    map["test_map"]                    = &Map::create_test_map;
    map["insert_hex"]                  = sol::resolve<Map&(HexSite)>(&Map::insert);
    map["insert_river"]                = sol::resolve<Map&(RiverSite)>(&Map::insert);
    map[sol::meta_function::to_string] = [](const Map& map) {
        std::stringstream ss;
        {
            cereal::JSONOutputArchive ar(ss);
            ar(map);
        }
        return ss.str();
    };
}

static void push_hex_type(sol::state& lua) {
    lua.new_enum("HexType", "field", HexType::field, "forest", HexType::forest);
    auto hex = lua.new_usertype<HexSite>(
        "HexSite", sol::constructors<HexSite(HexCoordinate, HexType)>());
    hex["type"] = &HexSite::type;
}

static void push_river_type(sol::state& lua) {
    lua.new_enum("RiverType", "small", RiverType::small, "stream", RiverType::stream);
    auto river = lua.new_usertype<RiverSite>(
        "RiverSite",
        sol::constructors<RiverSite(HexCoordinate, HexCoordinate, RiverType)>());
    river["type"] = &RiverSite::type;
}

void lua_push_functions(sol::state& lua) {
    push_coordinates(lua);
    push_hex_type(lua);
    push_river_type(lua);
    push_map(lua);
}
}  // namespace map