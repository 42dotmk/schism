#pragma once

#include "schism/Core/Events/Event.h"
#include "schism/Util/SingleSignal.h"

namespace Schism::Core {
class EventManager;
class Window;

class EventAdapterBase {
    public:
        virtual ~EventAdapterBase() = default;

        Util::SingleSignal<void(Event&&)> Listener;

    protected:
        virtual bool OnEvent(Event&& e) = 0;

        friend Window;
        friend EventManager;
};

}  // namespace Schism::Core
