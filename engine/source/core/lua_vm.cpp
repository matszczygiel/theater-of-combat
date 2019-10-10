#include "core/lua_vm.h"

#include <sstream>

#include "core/log.h"

namespace lua {
int exception_handler(lua_State* L,
                      sol::optional<const std::exception&> maybe_exception,
                      sol::string_view description) {
    std::stringstream msg("Handling Lua error ", std::ios_base::ate);
    if (maybe_exception) {
        const std::exception& ex = *maybe_exception;
        msg << "(exception) : " << ex.what();
        engine_debug(msg.str().c_str());
        // throw ex;
    } else {
        msg << "(description) : ";
        msg.write(description.data(), description.size());
        engine_debug(msg.str().c_str());
    }

    return sol::stack::push(L, description);
}

sol::protected_function_result error_handler(
    lua_State*, sol::protected_function_result result) {
        if(!result.valid()) {
            sol::error err = result;
            engine_error("Lua error: {}", err.what());
            throw err;
        }
        return result;
}

sol::state& get_state() noexcept {
    static sol::state lua{[]() {
        sol::state lua;
        lua.set_exception_handler(&exception_handler);
        return lua;
    }()};
    return lua;
}

}  // namespace lua
