#include "MouseEventHandler.h"
#include "schism/Core/Events/Event.h"
#include "schism/System/Log.h"

namespace Schism::Core
{
    MouseEventHandler::~MouseEventHandler()
    {
        Listener.disconnect_slot();
    }

    bool MouseEventHandler::OnEvent(Event&& e)
    {
        bool wasHandled = false;
        switch (e.GetEventType())
        {
            case MouseButtonDownEvent::GetStaticType():
                SC_CORE_INFO("MOUSE DOWN");
                HandleMouseDown(static_cast<MouseButtonDownEvent&&>(e));
                wasHandled = true;

            case MouseButtonReleasedEvent::GetStaticType():
                HandleMouseReleased(static_cast<MouseButtonReleasedEvent&&>(e));
                // we don't set handled here because we want the top handler to forward the event 
                // but now we just need to intercept it
            default:
                break;
        }

        return wasHandled;
    }

    void MouseEventHandler::HandleMouseDown(MouseButtonDownEvent&& e)
    {
        auto evt = static_cast<MouseButtonDownEvent&>(e);
        auto btnId= static_cast<int>(evt.GetKey());
        auto btn = &m_mouseButtons[btnId];

        if (!btn)
        {
            Listener(MouseButtonPressedEvent(
                evt.GetKey(),
                evt.GetPosition().x,
                evt.GetPosition().y
            ));

            *btn = true;
        }

        Listener(std::move(e));
    }

    void MouseEventHandler::HandleMouseReleased(MouseButtonReleasedEvent&& e)
    {
        auto evt = static_cast<MouseButtonReleasedEvent&>(e);
        auto btnId= static_cast<int>(evt.GetKey());

        m_mouseButtons[btnId] = false;
    }
}
