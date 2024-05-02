#pragma once

#include "schism/System/Log.h"

#include <entt/core/fwd.hpp>
#include <sol/make_reference.hpp>
#include <sol/raii.hpp>
#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include <sol/types.hpp>
#include <unordered_set>

namespace Schism::Scripting::Lua
{
    // can be used as a query or a bundle
    class Bundle
    {
    public:
        Bundle(const sol::table& self, entt::registry* registry, const sol::variadic_args &va, sol::this_state s)
            :   m_self(self),
                m_registry(registry)
        {
            for (auto&& obj : va)
            {
                const auto o = obj.get<sol::table>();

                if (!o.valid())
                {
                    SC_CORE_ERROR("Bundle type not valid, cannot convert to table {}", sol::type_name(s, obj.get_type()));
                    continue;
                }

                const auto typeIdFunc = o["type_id"].get<sol::function>();

                if (!typeIdFunc.valid())
                {
                    SC_CORE_ERROR("Bundle called with type that is not a component");
                    continue;
                }

                auto typeId = typeIdFunc().get<entt::id_type>();

                if (auto i = m_types.find(typeId); i != std::end(m_types))
                {
                    SC_CORE_WARN("Bundle type already added, ignoring");
                    continue;
                }
                auto name =  o["name"].get<std::string>();

                m_types.insert(typeId);
            }
        };

    public:
        static void Bind(sol::state& state, entt::registry& registry)
        {
            state.new_usertype<Bundle>("Bundle",
                sol::meta_function::construct,
                sol::factories(
                    [&registry](sol::table o, sol::this_state s, sol::variadic_args va)
                    {
                        return Bundle(o, &registry, va, s);
                    }
                    )
                );
        }

    private:
        std::unordered_set<entt::id_type> m_types;
        sol::table m_self;
        entt::registry* m_registry;
    };
}
