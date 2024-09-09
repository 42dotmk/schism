#include "Window.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#if defined(SCHISM_LINUX_WAYLAND)
#include <wayland-egl.h>
#endif

#include "Events/KeyEvents.h"
#include "Events/Keyboard.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"
#include "schism/System/Log.h"

namespace Schism::Core {

Window::Window() : m_created(false), m_WindowPtr(nullptr), m_Data() {}

Window::~Window() {
    glfwDestroyWindow(m_WindowPtr);
}

void Window::Create(int w, int h, const char* name,
                    Ref<EventAdapterBase> eventAdapter) {

    if (m_created) {
        SC_CORE_ERROR(
            "Trying to create a window, while this current window is already "
            "created");
        return;
    }

    if (!glfwInit()) {
        SC_CORE_TRACE("Couldn't initialize glfw");
    }

    glfwWindowHint(GLFW_VISIBLE, 1);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_WindowPtr = glfwCreateWindow(w, h, name, NULL, NULL);

    if (m_WindowPtr == NULL) {
        SC_CORE_TRACE("Couldn't create window!");
    }

    m_Data.Width = w;
    m_Data.Height = h;
    m_Data.eventAdapter = eventAdapter;

    glfwSetWindowUserPointer(m_WindowPtr, &m_Data);

    HookGLFWEventFunctions();
    SetNativeHandle();

    // this could possibly be an atomic
    m_created = true;
}

void Window::ProcessEvents() {
    // when we add multi window support this might have to be called once at the
    // application level
    glfwPollEvents();
}

void Window::Swap() const {
    glfwSwapBuffers(m_WindowPtr);
}

void Window::SetNativeHandle() {
#if defined(SCHISM_PLATFORM_LINUX)

#if defined(SCHISM_LINUX_WAYLAND)
    struct wl_surface* surface =
        (struct wl_surface*)glfwGetWaylandWindow(m_WindowPtr);

    SC_ASSERT(surface, "Cannot get native wayland window surface");

    m_nativeHandle = wl_egl_window_create(surface, m_Data.Width, m_Data.Height);
#elif defined(SCHISM_LINUX_X11)
    m_nativeHandle = (void*)(uintptr_t)glfwGetX11Window(m_WindowPtr);
#else
    SC_STATIC_FAIL("Not a supported window protocol")
#endif

#elif defined(SCHISM_PLATFORM_WINDOWS)
    m_nativeHandle = glfwGetWin32Window(m_WindowPtr);
#elif defined(SCHISM_PLATFORM_MAC)
    m_nativeHandle = glfwGetCocoaWindow(m_WindowPtr);
#endif
}

void Window::HookGLFWEventFunctions() {
    HookMouseEvents();
    HookKeyEvents();
    HookWindowEvents();
}

void Window::HookMouseEvents() {
    SC_ASSERT(m_Data.eventAdapter, "There is no attached EventManager");

    glfwSetScrollCallback(
        m_WindowPtr, [](GLFWwindow* win, double xoffset, double yoffset) {
            auto data =
                static_cast<detail::WindowData*>(glfwGetWindowUserPointer(win));
            data->eventAdapter->OnEvent(MouseScrollEvent(xoffset, yoffset));
        });

    glfwSetMouseButtonCallback(m_WindowPtr, [](GLFWwindow* win, int button,
                                               int action, int mods) {
        if (button < 0 || button > 2) {
            SC_CORE_ERROR(
                "Mouse button not supported! Mouse button recived {0}", button);
            return;
        }

        auto data =
            static_cast<detail::WindowData*>(glfwGetWindowUserPointer((win)));

        double xpos;
        double ypos;

        glfwGetCursorPos(win, &xpos, &ypos);

        Mouse::Button btn = static_cast<Mouse::Button>(button);

        if (action == GLFW_PRESS) {
            data->eventAdapter->OnEvent(MouseButtonDownEvent(btn, xpos, ypos));
        } else if (action == GLFW_RELEASE) {
            data->eventAdapter->OnEvent(
                MouseButtonReleasedEvent(btn, xpos, ypos));
        }
        SC_CORE_WARN("Action {}", action);
    });

    glfwSetCursorPosCallback(m_WindowPtr, [](GLFWwindow* win, double xpos,
                                             double ypos) {
        auto data =
            static_cast<detail::WindowData*>(glfwGetWindowUserPointer((win)));
        data->eventAdapter->OnEvent(MouseMoveEvent(xpos, ypos));
    });
}

void Window::HookWindowEvents() {
    SC_ASSERT(m_Data.eventAdapter, "There is no attached EventManager");
    glfwSetWindowSizeCallback(m_WindowPtr, [](GLFWwindow* win, int width,
                                              int height) {
        auto data =
            static_cast<detail::WindowData*>(glfwGetWindowUserPointer((win)));
        data->eventAdapter->OnEvent(WindowResizeEvent(width, height));
    });

    glfwSetWindowCloseCallback(m_WindowPtr, [](GLFWwindow* win) {
        auto data =
            static_cast<detail::WindowData*>(glfwGetWindowUserPointer((win)));
        data->eventAdapter->OnEvent(WindowCloseEvent());
    });
}

void Window::HookKeyEvents() {
    SC_ASSERT(m_Data.eventAdapter, "There is no attached EventManager");
    glfwSetKeyCallback(m_WindowPtr, [](GLFWwindow* win, int key, int scancode,
                                       int action, int mods) {
        auto data =
            static_cast<detail::WindowData*>(glfwGetWindowUserPointer((win)));
        if (action == GLFW_PRESS) {
            data->eventAdapter->OnEvent(KeyDownEvent((Keyboard::Key)key));
        } else if (action == GLFW_RELEASE) {
            data->eventAdapter->OnEvent(KeyReleasedEvent((Keyboard::Key)key));
        }
    });
}
}  // namespace Schism::Core
