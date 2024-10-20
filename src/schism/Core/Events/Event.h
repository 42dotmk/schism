#pragma once
#include <optional>
#include <type_traits>
#include <utility>

namespace Schism {
enum class EventType {
    None = 0,
    KeyPressed,
    KeyDown,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonDown,
    MouseButtonReleased,
    MouseMove,
    MouseScroll,
    WindowResize,
    WindowClose,
    WindowFocus,
    COUNT
};

enum class EventCategory { Keyboard, Mouse, Window, COUNT };

#define SC_EVENT(type, category)                               \
    [[nodiscard]] EventType GetEventType() const override {    \
        return type;                                           \
    }                                                          \
    static constexpr EventType GetStaticType() {               \
        return type;                                           \
    }                                                          \
    static constexpr EventCategory GetStaticCategory() {       \
        return category;                                       \
    }                                                          \
    [[nodiscard]] EventCategory GetCategory() const override { \
        return category;                                       \
    }

class Event {
    public:
        virtual ~Event() = default;
        virtual EventType GetEventType() const = 0;
        virtual EventCategory GetCategory() const = 0;
};

class EventHandler {
    public:
        EventHandler(Event& e) : m_Evt(e) {}

        // will handle a event returning the function return
        // rtn: pair(isHandler, functionReturn)
        template <typename T, typename F,
                  typename = typename std::enable_if<
                      std::is_base_of<Event, T>::value>::type>
        inline constexpr auto Handle(const F& func) {
            if (m_Evt.GetEventType() == T::GetStaticType()) {
                auto e = static_cast<T&>(m_Evt);
                using ret_type = decltype(func(e));

                if constexpr (std::is_same_v<ret_type, void>) {
                    func(e);
                    return std::make_pair(true, nullptr);
                } else {
                    return std::make_pair(true, func(e));
                }
            }
            return std::make_pair(false, nullptr);
        }

    private:
        Event& m_Evt;
};

#define CLASSEVENT(handler, type) handler.Handle<type>([this](type& e)
#define EVENTCB(handler, type) handler.Handle<type>([](type & e))
}  // namespace Schism
