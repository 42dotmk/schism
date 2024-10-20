#pragma once

#include "schism/System/Ptr.h"
#include "schism/Core/EventHandlers/EventAdapterBase.h"

namespace Schism::Core::detail {
struct WindowData {
        // don't know if this will be needed
        int Width;
        int Height;

        // this is definatelly needed
        Ref<EventAdapterBase> eventAdapter;
};
}  // namespace Schism::Core::detail
