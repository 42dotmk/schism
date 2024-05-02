#pragma once

#include <sol/sol.hpp>
#include <sol/state.hpp>

namespace Schism::Scripting::Lua
{
    template<typename T>
    concept LuaBindable = requires(T t, sol::state& state)
    {
        { T::Bind(state) };
    };
}
