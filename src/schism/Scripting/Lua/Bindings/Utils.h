#pragma once

#include <sol/sol.hpp>
#include <string>

namespace Schism::Scripting::Lua
{
    template<typename T>
    static inline constexpr std::optional<T> getFromObject(const sol::table& object, const std::string& name)
    {
        const auto& f = object[name].get<T>();

        if (!f.valid())
        {
            return std::nullopt;
        }

        return f;
    }
}
