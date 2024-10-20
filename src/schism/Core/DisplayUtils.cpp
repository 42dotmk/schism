#include <schism/Core/DisplayUtils.h>

#include <schism/Util/Signal.h>

#include <GLFW/glfw3.h>
#include "schism/Util/SignalConnection.h"

/*#include "schism/Util/SignalConnection.h"*/

namespace Schism::Core::DisplayUtils {

namespace detail {
class DisplayHandler {
    public:
        DisplayHandler() {
            glfwSetMonitorCallback(monitorCallback);
            refreshDisplays();
        }

        auto ListDisplays() { return m_displays; }

        Ref<Display> PrimaryDisplay() {
            auto* primary = glfwGetPrimaryMonitor();
            auto display = findDisplay(primary);

            if (!display) {
                refreshDisplays();

                display = findDisplay(primary);

                if (display) {
                    auto display = Ref<Display>(new Display(primary));
                    m_displays.push_back(display);
                    return display;
                }
            }

            return display;
        }

        Util::SignalConnection OnDisplayAttached(DisplayCallback&& callback) {
            return m_attached.connect(callback);
        }

        Util::SignalConnection OnDisplayRemoved(DisplayCallback&& callback) {
            return m_removed.connect(callback);
        }

    private:
        static void monitorCallback(GLFWmonitor* monitor, int event) {
            auto handler = reinterpret_cast<DisplayHandler*>(
                glfwGetMonitorUserPointer(monitor));

            if (event == GLFW_CONNECTED) {
                handler->AttachedDisplay(monitor);
            } else if (event == GLFW_DISCONNECTED) {
                handler->RemovedDisplay(monitor);
            }
        }

    private:
        Ref<Display> findDisplay(GLFWmonitor* monitor) {
            auto found = std::ranges::find_if(m_displays,
                                              [monitor](Ref<Display>& display) {
                                                  return *display == monitor;
                                              });

            return found != std::end(m_displays) ? *found : nullptr;
        }

        void AttachedDisplay(GLFWmonitor* monitor) {
            Ref<Display> display = findDisplay(monitor);

            if (!display) {
                display = Ref<Display>(new Display(monitor));
                m_displays.push_back(display);
            }

            auto active = display->Active();
            display->SetActive(display ? true : true);

            m_attached(display);
        }

        void RemovedDisplay(GLFWmonitor* monitor) {
            Ref<Display> display = findDisplay(monitor);

            if (!display) {
                SC_CORE_ERROR(
                    "Yeah, weird, removed display is not actually in our "
                    "store");
                return;
            }

            display->SetActive(false);

            std::erase_if(m_displays, [monitor](const Ref<Display>& d) {
                return *d == monitor;
            });
        }

        void refreshDisplays() {
            int count;
            GLFWmonitor** arr = glfwGetMonitors(&count);

            m_displays.clear();
            m_displays.reserve(count);

            for (int i = 0; i < count; i++) {
                auto* monitor = arr[i];

                glfwSetMonitorUserPointer(monitor, this);

                Ref<Display> display = Ref<Display>(new Display(monitor));

                m_displays.push_back(display);
            }
        };

    private:
        std::vector<Ref<Display>> m_displays;
        Util::Signal<void(Ref<Display>)> m_attached;
        Util::Signal<void(Ref<Display>)> m_removed;
};

// This can be done to avoid static initialization, use a manager for static object for
// manual allocation and deallocation
static Ptr<DisplayHandler> handler;

static const Ptr<DisplayHandler>& getHandler() {
    if (!handler) {
        handler = Ptr<DisplayHandler>();
    }

    return handler;
};

}  // namespace detail

std::vector<Ref<Display>> ListDisplays() {
    auto& h = detail::getHandler();
    return h->ListDisplays();
}

Ref<Display> PrimaryDisplay() {
    auto& h = detail::getHandler();
    return h->PrimaryDisplay();
}

Util::SignalConnection OnDisplayAttached(DisplayCallback&& callback) {
    auto& h = detail::getHandler();
    return h->OnDisplayAttached(std::move(callback));
}

Util::SignalConnection OnDisplayRemoved(DisplayCallback&& callback) {
    auto& h = detail::getHandler();
    return h->OnDisplayRemoved(std::move(callback));
}
}  // namespace Schism::Core::DisplayUtils
