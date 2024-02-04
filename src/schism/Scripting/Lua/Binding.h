#pragma once
#include <sol/sol.hpp>

namespace Schism::Scripting::Lua
{
    class Binding
    {
    public:
        Binding() = default;
        ~Binding() = default;
        virtual void Bind(sol::state& lua) = 0;
    };
}
