#ifndef LUA_GAMEPLAY_H
#define LUA_GAMEPLAY_H

#include <sol/state.hpp>

namespace gameplay {
void lua_push_functions(sol::state& lua);
}

#endif