#pragma once

#include "schism/Core/EventHandlers/EventAdapterBase.h"
#include "schism/Core/Events/MouseEvents.h"

#include <array>

namespace Schism::Core {
class MouseEventHandler : public EventAdapterBase {
    public:
        ~MouseEventHandler() override;

        bool OnEvent(Event&& e) override;

    private:
        void HandleMouseDown(MouseButtonDownEvent&& e);
        void HandleMouseReleased(MouseButtonReleasedEvent&& e);

        std::array<bool, 3> m_mouseButtons;
};

}  // namespace Schism::Core
