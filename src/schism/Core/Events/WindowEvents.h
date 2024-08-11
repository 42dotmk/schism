#pragma once

#include "Event.h"

namespace Schism
{
	template<EventType Type>
	class WindowEvent : public Event
	{
	public:
		SC_EVENT(Type, EventCategory::Window)
		WindowEvent() = default;
	};

	class WindowResizeEvent : public WindowEvent<EventType::WindowResize>
	{
	public:
		WindowResizeEvent(int w, int h)
			:
			m_Width(w),
			m_Height(h)
		{}

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
	private:
		int m_Width;
		int m_Height;
	};

	using WindowCloseEvent = WindowEvent<EventType::WindowClose>;
	using WindowFocusEvent = WindowEvent<EventType::WindowFocus>;
}

