#pragma once

#include <entt/entt.hpp>
#include <schism/Interfaces/IScene.h>
#include <schism/Scripting/Lua/Binding.h>

namespace Schism
{
	class Entity: public Scripting::Lua::Binding
	{
	public:
		Entity(IScene* scene, entt::entity id)
			:
			m_Scene(scene),
			m_Id(id)
		{
		}

		Entity(Entity& ent) = default;

		template<typename T, typename ...Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->Registry().emplace<T>(m_Id, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->Registry().get<T>(m_Id);
		}

		template<typename T>
		bool HasComponent()
		{
            return m_Scene->Registry().any_of<T>(m_Id);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->Registry().remove<T>(m_Id);
		}

        void Update(float dt)
        {
            // call the lua update function
        }

        void Bind(sol::state& lua)
        {
            // bind the object here
            // also script file here
        }
    private:
        void OnUpdate(float dt)
        {

        }
	private:
		entt::entity m_Id;
		IScene* m_Scene;
	};
}
