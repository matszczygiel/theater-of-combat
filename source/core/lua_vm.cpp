#include "lua_vm.h"

#include <sstream>

#include "log.h"

namespace lua {
int exception_handler(lua_State* L,
                      sol::optional<const std::exception&> maybe_exception,
                      sol::string_view description) {
    std::stringstream msg("Handling Lua error ", std::ios_base::ate);
    if (maybe_exception) {
        const std::exception& ex = *maybe_exception;
        msg << "(exception) : " << ex.what();
        engine_debug(msg.str().c_str());
        throw ex;
    } else {
        msg << "(description) : ";
        msg.write(description.data(), description.size());
        engine_debug(msg.str().c_str());
    }

    return sol::stack::push(L, description);
}

sol::state& get_state() {
    static sol::state lua{[]() {
        sol::state lua;
        lua.set_exception_handler(&exception_handler);
        return lua;
    }()};
    return lua;
}

}  // namespace lua
