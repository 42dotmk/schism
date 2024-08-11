#pragma once
#include "Event.h"
#include "Keyboard.h"

namespace Schism
{
	template<EventType Type>
	class KeyEvent : public Event
	{
	public:
		SC_EVENT(Type, EventCategory::Keyboard)
	    KeyEvent(Keyboard::Key key) : m_Key(key) {}
		Keyboard::Key GetKey() const { return m_Key; }
	private:
		Keyboard::Key m_Key;
	};

	using KeyDownEvent = KeyEvent<EventType::KeyDown>;
	using KeyPressedEvent = KeyEvent<EventType::KeyPressed>;
	using KeyReleasedEvent = KeyEvent<EventType::KeyReleased>;
}
