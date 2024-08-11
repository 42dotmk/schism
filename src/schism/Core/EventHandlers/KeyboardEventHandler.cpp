#include "KeyboardEventHandler.h"

#include "schism/System/Log.h"

namespace Schism::Core
{
    KeyboardEventHandler::KeyboardEventHandler()
    {
        m_pressedKeys.fill(false);
    }

    KeyboardEventHandler::~KeyboardEventHandler()
    {
        Listener.disconnect_slot();
    }

    bool KeyboardEventHandler::OnEvent(Event&& e)
    {
        bool wasHandled = true;

        switch (e.GetEventType()) 
        {
            case KeyDownEvent::GetStaticType():
                wasHandled = HandleKeyDown(static_cast<KeyDownEvent&&>(e));
                break;

            case KeyReleasedEvent::GetStaticType():
                wasHandled = HandleKeyRelease(static_cast<KeyReleasedEvent&&>(e));
                break;

            default:
                break;
        }

        return wasHandled;
    }

    bool KeyboardEventHandler::HandleKeyDown(KeyDownEvent&& e)
    {
        auto keyId = static_cast<int>(e.GetKey());

        if (keyId < 0 || keyId > m_pressedKeys.size())
        {
            SC_CORE_WARN("Got key that was not in the registry {}", keyId);
            return true;
        }

        auto& key = m_pressedKeys[keyId];

        if (!key)
        {
            key = true;
            
            Listener(KeyPressedEvent(e.GetKey()));
        }

        Listener(e);

        return true;
    }

    bool KeyboardEventHandler::HandleKeyRelease(KeyReleasedEvent&& e)
    {
        auto keyId = static_cast<int>(e.GetKey());

        if (keyId < 0 || keyId > m_pressedKeys.size())
        {
            SC_CORE_WARN("Got key that was not in the registry {}", keyId);
            return true;
        }

        auto& key = m_pressedKeys[keyId]; 

        if (key)
        {
            key = false;
            Listener(e);
            return true;
        }

        return false;
    }
}
