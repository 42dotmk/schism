#pragma once

#include <schism/Core/SharedContext.h>
#include <schism/Core/Timestep.h>
#include <schism/Scripting/Lua/Lua.h>

#include <string>

namespace Schism
{
	class SceneBase
	{
	public:
		SceneBase(Core::SharedContextRef ctx, const std::string& name);
		virtual ~SceneBase();

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnPause() = 0;
		virtual void OnResume() = 0;
		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnDraw() = 0;
		virtual void OnSystemEvent(Event& e) = 0;

        // might not be needed to be exposed from the Scene
		entt::registry& Registry();

		const std::string& GetName() const { return m_Name; }
		bool WasPaused() const { return m_WasPaused; }

    protected:
        // This should only be temporary
        template<Scripting::Lua::LuaBindable Component, typename ...Args>
        Component& EmplaceComponent(entt::entity entity, Args... args)
        {
            return m_Registry.emplace<Component>(entity, std::forward<Args...>(args...));
        }

	protected:
		bool m_WasPaused{ false };
		std::string m_Name;
		Core::SharedContextRef m_Ctx;
		entt::registry m_Registry;
	};
}
