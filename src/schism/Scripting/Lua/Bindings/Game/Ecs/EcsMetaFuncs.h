#pragma once

#include <entt/core/hashed_string.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <entt/meta/factory.hpp>
#include <sol/make_reference.hpp>
#include <sol/state.hpp>

namespace Schism::Ecs::detail::metafuncs
{
    template<typename Component>
    auto emplace(entt::registry* registry, sol::this_state s, entt::entity entity)
    {
        auto& comp = registry->emplace<Component>(entity);

        return sol::make_reference(s, std::ref(comp));
    }

    template<typename T>
    void get(entt::registry* registry, sol::this_state s, entt::entity entity)
    {
        auto& comp = registry->get<T>(entity);

        return sol::make_reference(s, std::ref(comp));
    }

    template<typename T>
    bool has(entt::registry* registry, entt::entity entity)
    {
        return registry->any_of<T>(entity);
    }

    template<typename Component>
    void RegisterComponentMetaFunc()
    {
        using namespace entt::literals;

        entt::meta<Component>()
            .template func<&emplace<Component>>("emplace"_hs)
            .template func<&get<Component>>("get"_hs)
            .template func<&has<Component>>("has"_hs);
    }
}
