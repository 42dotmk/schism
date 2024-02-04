#pragma once
#include <type_traits>
#include <iostream>
namespace Schism
{
	enum class EventType
	{
		None = 0,
		KeyPressed, KeyDown, KeyReleased,
		MouseButtonPressed, MouseButtonDown, MouseButtonReleased, MouseMove, MouseScroll,
		WindowResize, WindowClose, WindowFocus
	};

#define SC_EVENT(type) [[nodiscard]] EventType GetEventType() const override { return type; } \
						static constexpr EventType GetStaticType() { return type; }

	class Event
	{
	public:
		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
	};

	class EventHandler
	{
	public:
		EventHandler(Event& e)
			:
			m_Evt(e) {}

		template<typename T, typename F,
			typename = typename std::enable_if<std::is_base_of<Event, T>::value>::type>
			constexpr bool Handle(const F& func)
		{	
			if (m_Evt.GetEventType() == T::GetStaticType())
			{
				func(static_cast<T&>(m_Evt));
				return true;
			}
			return false;
		}
	private:
		Event& m_Evt;
	};

#define CLASSEVENT(handler, type) handler.Handle<type>([this](type& e)
#define EVENTCB(handler, type) handler.Handle<type>([](type& e))
}
