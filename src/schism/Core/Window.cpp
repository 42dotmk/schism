#include "Window.h"

#include "Events/KeyEvents.h"
#include "Events/Keyboard.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"
#include "glad/glad.h"
#include "schism/System/Log.h"

namespace Schism::Core {

Window::Window() : m_WindowPtr(nullptr), m_LoadWinPtr(nullptr), m_Data() {}

Window::~Window() {
    glfwDestroyWindow(m_LoadWinPtr);
    glfwDestroyWindow(m_WindowPtr);
}

void Window::Create(int w, int h, const char* name) {
    if (!glfwInit()) {
        SC_CORE_TRACE("Couldn't initialize glfw");
    }

    glfwWindowHint(GLFW_VISIBLE, 0);
    m_LoadWinPtr = glfwCreateWindow(1, 1, "Loading Context", NULL, NULL);

    if (m_LoadWinPtr == NULL) {
        SC_CORE_TRACE("Couldn't create loading context!");
    }

    glfwWindowHint(GLFW_VISIBLE, 1);
    m_WindowPtr = glfwCreateWindow(w, h, name, NULL, m_LoadWinPtr);

    if (m_WindowPtr == NULL) {
        SC_CORE_TRACE("Couldn't create window!");
    }

    glfwMakeContextCurrent(m_WindowPtr);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SC_CORE_TRACE("Couldn't load glad!");
    }

    glfwSetWindowUserPointer(m_WindowPtr, &m_Data);

    m_Data.Width = w;
    m_Data.Height = h;

    HookGLFWEventFunctions();
}

void Window::ProcessEvents() {
    glfwPollEvents();
}

void Window::AttachEventAdapter(Ref<EventAdapterBase> eventAdapter) {
    m_Data.eventAdapter = eventAdapter;
}

void Window::Swap() const {
    glfwSwapBuffers(m_WindowPtr);
}

void Window::HookGLFWEventFunctions() {
    HookMouseEvents();
    HookKeyEvents();
    HookWindowEvents();
}

void Window::HookMouseEvents() {
    glfwSetScrollCallback(
        m_WindowPtr, [](GLFWwindow* win, double xoffset, double yoffset) {
            auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(win));
            SC_ASSERT(data->eventAdapter, "There is no attached EventAdapter");
            data->eventAdapter->OnEvent(MouseScrollEvent(xoffset, yoffset));
        });

    glfwSetMouseButtonCallback(m_WindowPtr, [](GLFWwindow* win, int button,
                                               int action, int mods) {
        if (button < 0 || button > 2) {
            SC_CORE_ERROR(
                "Mouse button not supported! Mouse button recived {0}", button);
            return;
        }

        auto data = static_cast<WindowData*>(glfwGetWindowUserPointer((win)));

        SC_ASSERT(data->eventAdapter, "There is no attached EventAdapter");

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
        auto data = static_cast<WindowData*>(glfwGetWindowUserPointer((win)));
        SC_ASSERT(data->eventAdapter, "There is no attached EventAdapter");
        data->eventAdapter->OnEvent(MouseMoveEvent(xpos, ypos));
    });
}

void Window::HookWindowEvents() {
    glfwSetWindowSizeCallback(m_WindowPtr, [](GLFWwindow* win, int width,
                                              int height) {
        auto data = static_cast<WindowData*>(glfwGetWindowUserPointer((win)));
        SC_ASSERT(data->eventAdapter, "There is no attached EventAdapter");
        data->eventAdapter->OnEvent(WindowResizeEvent(width, height));
    });

    glfwSetWindowCloseCallback(m_WindowPtr, [](GLFWwindow* win) {
        auto data = static_cast<WindowData*>(glfwGetWindowUserPointer((win)));
        SC_ASSERT(data->eventAdapter, "There is no attached EventAdapter");
        data->eventAdapter->OnEvent(WindowCloseEvent());
    });
}

void Window::HookKeyEvents() {
    glfwSetKeyCallback(m_WindowPtr, [](GLFWwindow* win, int key, int scancode,
                                       int action, int mods) {
        auto data = static_cast<WindowData*>(glfwGetWindowUserPointer((win)));
        SC_ASSERT(data->eventAdapter, "There is no attached EventAdapter");
        if (action == GLFW_PRESS) {
            data->eventAdapter->OnEvent(KeyDownEvent((Keyboard::Key)key));
        } else if (action == GLFW_RELEASE) {
            data->eventAdapter->OnEvent(KeyReleasedEvent((Keyboard::Key)key));
        }
    });
}
}  // namespace Schism::Core
