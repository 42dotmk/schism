#include "EventManager.h"

namespace Schism::Core {
void EventManager::AttachAdapter(EventCategory category,
                                 Ref<EventAdapterBase> handler) {
    const auto cat = static_cast<std::size_t>(category);
    m_Handlers[cat] = handler;

    handler->Listener.connect([this](Event&& e) { Listener(e); });
}

bool EventManager::OnEvent(Event&& e) {
    const auto cat = static_cast<std::size_t>(e.GetCategory());

    const auto& handler = m_Handlers[cat];

    bool wasHandled = false;

    if (handler) {
        wasHandled = handler->OnEvent(std::move(e));
    }

    if (!wasHandled) {
        Listener(std::move(e));
        wasHandled = true;
    }

    return wasHandled;
}
}  // namespace Schism::Core
