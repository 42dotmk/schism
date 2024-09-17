#pragma once

#include "schism/Core/EventHandlers/EventAdapterBase.h"
#include "schism/Core/WindowData.h"
#include "schism/System/Ptr.h"

#include "GLFW/glfw3.h"

// This is the main window class, once we start to support wasm, android, this will have to be an interface or something
namespace Schism::Core {
using WindowRef = Ref<Window>;

class Window {
    public:
        using NativeHandle = void*;
        using GLFWwindowHandle = GLFWwindow*;

    public:
        Window();
        virtual ~Window();

        void Create(int w, int h, const char* name,
                    Ref<EventAdapterBase> eventAdapter);

        void SwapEventAdapter(Ref<EventAdapterBase> eventAdapter);

        void Resize();

        int GetWidth() const { return m_Data.Width; }

        int GetHeight() const { return m_Data.Height; }

        void Swap() const;
        void ProcessEvents();

        // Temporary, shouldn't expose glfw window, or maybe it should, idk
        GLFWwindow* GetGLFWWindow() const { return m_WindowPtr; }

        NativeHandle GetNativeDisplay() const { return m_nativeDisplay; };

        NativeHandle GetNativeHandle() const { return m_nativeHandle; }

    private:
        void SetNativeHandle();

        // Event Handling
        void HookGLFWEventFunctions();
        void HookMouseEvents();
        void HookWindowEvents();
        void HookKeyEvents();

        bool m_created;
        NativeHandle m_nativeHandle;
        NativeHandle m_nativeDisplay;
        GLFWwindowHandle m_WindowPtr;

        detail::WindowData m_Data;
};
}  // namespace Schism::Core
