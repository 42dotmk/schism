#pragma once

#include "schism/Core/Events/Event.h"
#include "schism/Core/EventHandlers/EventAdapterBase.h"
#include "schism/System/Ptr.h"

namespace Schism::Core
{
    class Window;

    class EventManager: public EventAdapterBase
    {
    public:

        void AttachAdapter(EventCategory category, Ref<EventAdapterBase> handler);

    private:
        bool OnEvent(Event&& e) override;

        static constexpr int handlerCount = static_cast<std::size_t>(EventCategory::COUNT);
        std::array<Ref<EventAdapterBase>, handlerCount> m_Handlers;

        friend Window;
    };

}
