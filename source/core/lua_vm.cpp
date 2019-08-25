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
        // throw ex;
    } else {
        msg << "(description) : ";
        msg.write(description.data(), description.size());
        engine_debug(msg.str().c_str());
    }

    return sol::stack::push(L, description);
}
/*
sol::protected_function_result error_handler(
    lua_State* L, sol::protected_function_result result) {
    sol::type t          = sol::type_of(L, result.stack_index());
    std::string err = "sol: ";
    err += sol::to_string(result.status());
    err += " error";

    if (t == sol::type::string) {
        err += ": ";
        std::string_view serr =
            sol::stack::unqualified_get<std::string_view>(L, result.stack_index());
        err.append(serr.data(), serr.size());
    }

#if defined(SOL_PRINT_ERRORS) && SOL_PRINT_ERRORS
    std::cerr
        << "[sol3] An error occurred and has been passed to an error handler: ";
    std::cerr << err;
    std::cerr << std::endl;
#endif
    // replacing information of stack error into pfr
    int target = result.stack_index();
    if (result.pop_count() > 0) {
        sol::stack::remove(L, target, result.pop_count());
    }
    sol::stack::push(L, err);
    int top     = lua_gettop(L);
    int towards = top - target;
    if (towards != 0) {
        lua_rotate(L, top, towards);
    }
#if defined(SOL_NO_EXCEPTIONS) && SOL_NO_EXCEPTIONS
    return result;
#else
    // just throw our error
    throw sol::error(sol::detail::direct_error, err);
#endif  // If exceptions are allowed
}
}  // namespace lua
*/

sol::protected_function_result error_handler(
    lua_State* L, sol::protected_function_result result) {
        try {
            sol::script_throw_on_error(L, result);
        }
        catch(sol::error& err) {
            engine_debug("Lua error: {}", err.what());
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
