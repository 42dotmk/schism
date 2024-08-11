#include "SceneManager.h"

namespace Schism::Core
{
	SceneManager::SceneManager() = default;
	
	SceneManager::SceneManager(SharedContextRef ctx)
		:
		m_Ctx(ctx)
	{
	}

	void SceneManager::InitContext(SharedContextRef ctx)
	{
		m_Ctx = ctx;
	}

	void SceneManager::Switch(const std::string& name)
	{
		if (auto i = m_Scenes.find(name); i != m_Scenes.end())
		{
			m_ActiveScene->OnPause();
			m_ActiveScene = i->second;
			if (m_ActiveScene->WasPaused())
			{
				m_ActiveScene->OnResume();
			}
			return;
		}

		SC_CORE_TRACE("Cannot find scene: {0}", name);
	}

	void SceneManager::Destroy(const std::string& name)
	{
		if (auto i = m_Scenes.find(name); i != m_Scenes.end())
		{
			if (m_ActiveScene == i->second)
			{
				SC_CORE_WARN("Cannot destroy the active scene");
				return;
			}
			m_Scenes.erase(i);
			return;
		}

		SC_CORE_TRACE("Cannot find scene: {0}", name);
	}
	
	void SceneManager::OnSystemEvent(Event& e) const
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->OnSystemEvent(e);
		}
	}

	
	void SceneManager::OnUpdate(Timestep ts) const
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->OnUpdate(ts);
		}
	}

	void SceneManager::OnDraw() const
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->OnDraw();
		}
	}
}
