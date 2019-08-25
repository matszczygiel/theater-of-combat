#ifndef LUA_VM_H
#define LUA_VM_H

#include <sol/sol.hpp>

namespace lua {
sol::state& get_state();
}

#endif