#ifndef LUA_VM_H
#define LUA_VM_H

#include <sol/sol.hpp>

namespace lua {
sol::state& get_state() noexcept;
sol::protected_function_result error_handler(
    lua_State* L, sol::protected_function_result result);
}  // namespace lua

#endif