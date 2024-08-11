#pragma once
#include "SceneManager.h"
#include "SharedContext.h"
#include "schism/Core/EventHandlers/EventManager.h"
#include "schism/Core/Events/Event.h"

namespace Schism {
	
	class Application
	{
	public:
		Application(int w, int h, const char* name);
		virtual ~Application();

		void Run();
	private:
		void OnEvent(Event& e);

        void SetupEventHandlers();
	protected:
		Core::SharedContextRef m_Ctx;
		Core::SceneManager m_SceneManager;
        Ref<Core::EventManager> m_EventManager;
	};
}
