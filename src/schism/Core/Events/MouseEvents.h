#pragma once
#include <glm/vec2.hpp>

#include "Event.h"
#include "Mouse.h"
namespace Schism
{
	template<EventType Type>
	class MouseEvent : public Event
	{
	public:
		SC_EVENT(Type)
			MouseEvent(float x, float y)
			:
			m_Pos(x, y)
		{}

		MouseEvent(Mouse::Button btn, float x, float y)
			:
			m_Btn(btn),
			m_Pos(x, y)
		{}

		Mouse::Button GetKey() const { return m_Btn; }
		const glm::vec2& GetPosition() const { return m_Pos; }
	private:
		Mouse::Button m_Btn{ Mouse::Button::NONE };
		glm::vec2 m_Pos;
	};

	using MouseMoveEvent = MouseEvent<EventType::MouseMove>;
	using MouseScrollEvent = MouseEvent<EventType::MouseScroll>;
	using MouseButtonDownEvent = MouseEvent<EventType::MouseButtonDown>;
	using MouseButtonPressedEvent = MouseEvent<EventType::MouseButtonPressed>;
	using MouseButtonReleasedEvent = MouseEvent<EventType::MouseButtonReleased>;
}
