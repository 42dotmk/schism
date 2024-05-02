#pragma once

#include <sol/raii.hpp>
#include <sol/sol.hpp>
#include <sol/types.hpp>

namespace Schism::Scripting::Lua
{
    class Plugin
    {
        enum Where: std::uint32_t 
        {
            Startup,
            Update,
            Destory,
        };

    public:
        Plugin(const sol::table& table)
        {
            auto ctor = table["init"];

            auto f = ctor.get<sol::function>();

            if (f.valid())
            {
                f();
            }
        }
    public:

        static void Bind(sol::state& state)
        {
            state.new_usertype<Plugin>(
                    "Plugin",
                    sol::meta_function::construct,
                    sol::factories(
                        [](sol::table o)
                        {
                            return Plugin(o);
                        }
                    )
                    );
        }

    private:
        sol::table t;
    };
}
