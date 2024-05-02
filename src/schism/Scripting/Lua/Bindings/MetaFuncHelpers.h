#pragma once

#include <entt/core/fwd.hpp>
#include <entt/core/hashed_string.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>
#include <sol/forward.hpp>
#include <sol/table.hpp>
#include <utility>

namespace Schism::Scripting::Lua
{
    template<typename ...Args>
    inline auto InvokeEcsMetaFunc(const sol::table& table, entt::id_type functionId, Args&& ...args) 
    {
        // for now we would only be using the typeid
        const auto& typeIdFunc = table["type_id"].get<sol::function>();

        if (!typeIdFunc.valid())
        {
            return entt::meta_any{};
        }

        auto typeId = typeIdFunc().get<entt::id_type>();
        auto metaType = entt::resolve(typeId);

        if (auto&& metaFunc = metaType.func(functionId); metaFunc)
        {
            return metaFunc.invoke({}, std::forward<Args>(args)...);
        }
        else
        {
            return entt::meta_any{};
        }
    }
}

