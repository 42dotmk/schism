#pragma once

#include "schism/Core/EventHandlers/EventAdapterBase.h"
#include "schism/System/Ptr.h"

#include "GLFW/glfw3.h"

namespace Schism::Core
{
	class Window
	{
	public:
		Window();
		virtual ~Window();

		int GetWidth() const { return m_Data.Width; }
		int GetHeight() const { return m_Data.Height; }

		void Swap() const;
		void Create(int w, int h, const char* name);
		void ProcessEvents();
        void AttachEventAdapter(Ref<EventAdapterBase> eventManager);

		// Temporary, shouldn't expose glfw window
		GLFWwindow* GetNativeWindow() const { return m_WindowPtr; }
		GLFWwindow* GetLoadingContext() const { return m_LoadWinPtr; }
	private:
		void HookGLFWEventFunctions();
		void HookMouseEvents();
		void HookWindowEvents();
		void HookKeyEvents();
		
		GLFWwindow* m_WindowPtr;
		GLFWwindow* m_LoadWinPtr;

		struct WindowData {
            // don't know if this will be needed
            int Width;
            int Height;

            // this is definatelly needed
            Ref<EventAdapterBase> eventAdapter;
        } m_Data;
	};
}
