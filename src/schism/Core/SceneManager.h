#pragma once

#include <parallel_hashmap/phmap.h>

#include "Timestep.h"
#include "Events/Event.h"
#include "schism/Interfaces/IScene.h"
#include "schism/System/System.h"

namespace Schism::Core
{	
	class SceneManager
	{
	public:
		SceneManager();
		SceneManager(SharedContextRef ctx);

		void InitContext(SharedContextRef ctx);
		
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<IScene, T>>>
		void Register(const std::string& name)
		{
			if (auto i = m_Scenes.find(name); i != m_Scenes.end())
			{
				SC_CORE_TRACE("Scene already registered: {0}", name);
				return;
			}
			
			Ref<IScene> Scene = MakeRef<T>(m_Ctx, name);
			Scene->OnAttach();
			m_Scenes[name] = Scene;

			SC_CORE_INFO("Registered Scene {0}", name);
			
			if (!m_ActiveScene)
			{
				m_ActiveScene = Scene;
			}
		}
		
		void Switch(const std::string& name);
		void Destroy(const std::string& name);
		
		void OnUpdate(Timestep ts) const;
		void OnDraw() const;
		void OnSystemEvent(Event& e) const;

		Ref<IScene> GetActiveScene() const { return m_ActiveScene; }
	private:
		SharedContextRef m_Ctx;
		Ref<IScene> m_ActiveScene { nullptr };
		phmap::node_hash_map<std::string, Ref<IScene>> m_Scenes;
	};
}
