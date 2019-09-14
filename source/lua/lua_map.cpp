#include "lua_map.h"

#include <sol/sol.hpp>

#include "core/lua_vm.h"
#include "map/hexagons.h"
#include "map/map.h"
#include "map/types.h"

namespace map {
void lua_push_functions() {
    auto& lua = lua::get_state();

    auto coord = lua.new_usertype<HexCoordinate>(
        "Coord", sol::constructors<HexCoordinate(), HexCoordinate(int, int)>());
    coord["q"] = &HexCoordinate::q;
    coord["p"] = &HexCoordinate::p;

    lua.new_enum("HexType", "field", HexType::field, "forest", HexType::forest);

    auto hex = lua.new_usertype<HexSite>(
        "HexSite", sol::constructors<HexSite(HexCoordinate, HexType)>());
    hex["type"] = &HexSite::type;

    lua.new_enum("RiverType", "small", RiverType::small, "stream",
                 RiverType::stream);

    auto river = lua.new_usertype<RiverSite>(
        "RiverSite", sol::constructors<RiverSite(HexCoordinate, HexCoordinate,
                                                 RiverType)>());
    river["type"] = &RiverSite::type;


    auto map = lua.new_usertype<Map>(
        "Map", sol::constructors<Map()>());
    map["test_map"] = &Map::create_test_map;
    map["insert_hex"] = sol::resolve<void(HexSite)>(&Map::insert);
    map["insert_river"] = sol::resolve<void(RiverSite)>(&Map::insert);
}
}  // namespace map