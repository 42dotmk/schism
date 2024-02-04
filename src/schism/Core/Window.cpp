#include "Window.h"

#include "Events/Keyboard.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"
#include "glad/glad.h"
#include "schism/System/Log.h"
#include <GLFW/glfw3.h>

namespace Schism::Core
{
	Window::Window() : m_WindowPtr(nullptr), m_LoadWinPtr(nullptr), m_Data()
	{
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_WindowPtr);
	}

	void Window::Create(int w, int h, const char* name)
	{
		if (!glfwInit())
		{
			SC_CORE_TRACE("Couldn't initialize glfw");
		}
		
		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
		m_LoadWinPtr = glfwCreateWindow(1, 1, "Loading Context", NULL, NULL);

		if (m_LoadWinPtr == NULL)
		{
			SC_CORE_TRACE("Couldn't create loading context!");
		}

		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
		m_WindowPtr = glfwCreateWindow(w, h, name, NULL, m_LoadWinPtr);

		if (m_WindowPtr == NULL)
		{
			SC_CORE_TRACE("Couldn't create window!");
		}

		glfwMakeContextCurrent(m_WindowPtr);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			SC_CORE_TRACE("Couldn't load glad!");
		}

		glfwSetWindowUserPointer(m_WindowPtr, &m_Data);

		m_Data.Width = w;
		m_Data.Height = h;

		glfwSwapInterval(1);
		
		HookGLFWEventFunctions();
	}

	void Window::ProcessEvents()
	{
		glfwPollEvents();
	}

	void Window::SetEventCallback(EventCallback callback)
	{
		m_Data.OnEvent = callback;
	}
	
	void Window::Swap() const
	{
		glfwSwapBuffers(m_WindowPtr);
	}

	void Window::HookGLFWEventFunctions()
	{
		HookMouseEvents();
		HookKeyEvents();
		HookWindowEvents();
	}
	
	void Window::HookMouseEvents()
	{
		glfwSetScrollCallback(m_WindowPtr, [](GLFWwindow* win, double xoffset, double yoffset)
			{
				auto data = static_cast<WindowData*>(glfwGetWindowUserPointer((win)));
				data->OnEvent(MouseScrollEvent(xoffset, yoffset));
			});

		glfwSetMouseButtonCallback(m_WindowPtr, [](GLFWwindow* win, int button, int action, int mods)
			{
				if (button < 0 || button > 2)
				{
					SC_CORE_ERROR("Mouse button not supported! Mouse button recived {0}", button);
					return;
				}

				auto data = static_cast<WindowData*>(glfwGetWindowUserPointer((win)));

				static bool* MouseButtonStates[] = {
					&data->Input.MouseButton.Left,
					&data->Input.MouseButton.Right,
					&data->Input.MouseButton.Middle,
				};

				double xpos;
				double ypos;

				glfwGetCursorPos(win, &xpos, &ypos);

				data->Input.MousePosition = { xpos, ypos };

				const auto MouseBtn = (Mouse::Button)(button);
				bool* MouseButtonState = MouseButtonStates[button];

				if (action == GLFW_PRESS)
				{
					if (!*MouseButtonState)
					{
						data->OnEvent(MouseButtonPressedEvent(
							MouseBtn,
							data->Input.MousePosition.x,
							data->Input.MousePosition.y
						));
						(*MouseButtonState) = true;
					}

					data->OnEvent(MouseButtonDownEvent(
						MouseBtn,
						data->Input.MousePosition.x,
						data->Input.MousePosition.y
					));

				}
				else if (*MouseButtonState && action == GLFW_RELEASE)
				{
					(*MouseButtonState) = false;
					data->OnEvent(MouseButtonReleasedEvent(
						MouseBtn,
						data->Input.MousePosition.x,
						data->Input.MousePosition.y
					));
				}

			});

		glfwSetCursorPosCallback(m_WindowPtr, [](GLFWwindow* win, double xpos, double ypos)
			{
				auto data = static_cast<WindowData*>(glfwGetWindowUserPointer((win)));
				data->Input.MousePosition = { xpos, ypos };
				data->OnEvent(MouseMoveEvent(xpos, ypos));
			});
	}

	void Window::HookWindowEvents()
	{
		glfwSetWindowSizeCallback(m_WindowPtr, [](GLFWwindow* win, int width, int height)
			{
				auto data = static_cast<WindowData*>(glfwGetWindowUserPointer((win)));
				data->OnEvent(WindowResizeEvent(width, height));
				data->Width = width;
				data->Height = height;
			});

		glfwSetWindowCloseCallback(m_WindowPtr, [](GLFWwindow* win)
			{
				auto data = static_cast<WindowData*>(glfwGetWindowUserPointer((win)));
				data->OnEvent(WindowCloseEvent());
			});
	}
	
	void Window::HookKeyEvents()
	{
		glfwSetKeyCallback(m_WindowPtr, [](GLFWwindow* win, int key, int scancode, int action, int mods)
			{
				auto data = static_cast<WindowData*>(glfwGetWindowUserPointer((win)));
				if (action == GLFW_PRESS)
				{
					if (!data->Input.PressedKeys[key])
					{
						data->Input.PressedKeys[key] = true;
						data->OnEvent(KeyPressedEvent((Keyboard::Key)key));
					}

					data->OnEvent(KeyDownEvent((Keyboard::Key)key));
				}
				else if (data->Input.PressedKeys[key] && action == GLFW_RELEASE)
				{
					data->Input.PressedKeys[key] = false;
					data->OnEvent(KeyReleasedEvent((Keyboard::Key)key));
				}
			});
	}
}
