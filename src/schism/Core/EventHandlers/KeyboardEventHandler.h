#pragma once

#include "schism/Core/EventHandlers/EventAdapterBase.h"

#include "schism/Core/Events/KeyEvents.h"

#include <array>

namespace Schism::Core {

class KeyboardEventHandler : public EventAdapterBase {
    public:
        KeyboardEventHandler();
        ~KeyboardEventHandler() override;

        bool OnEvent(Event&& e) override;
        ;

    private:
        bool HandleKeyDown(KeyDownEvent&& e);
        bool HandleKeyRelease(KeyReleasedEvent&& e);

        std::array<bool, 400> m_pressedKeys;
};
}  // namespace Schism::Core
