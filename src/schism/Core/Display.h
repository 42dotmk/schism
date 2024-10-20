#pragma once

#include <GLFW/glfw3.h>

#include <schism/System/Ptr.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace Schism::Core {

namespace DisplayUtils::detail {
class DisplayHandler;
}

// this can be abstracted away into a base class and then implement the platform dependent stuff to avoid fwd declaration
// into other classes but meh...
class Display {

        friend DisplayUtils::detail::DisplayHandler;

    public:
        struct VideoMode {
                int width;
                int height;
                int redBits;
                int greenBits;
                int blueBits;
                int refreshRate;
        };

    public:
        void* getNativeHandle();

        std::string Name();
        bool Active();
        glm::vec2 PhysicalSize();
        glm::vec2 ContentScale();
        glm::vec2 VirtualPosition();
        glm::vec4 WorkArea();

        std::vector<struct VideoMode> VideoModes();
        VideoMode VideoMode();

        bool operator==(const Display& display);

        bool operator==(Ref<Display> const& display);

    private:
        Display(GLFWmonitor* monitor);

        void SetActive(bool isActive);
        bool operator==(GLFWmonitor* monitor);

        template <typename T>
        inline glm::vec2 vec2Getter(void (*f)(GLFWmonitor*, T*, T*)) {
            T x;
            T y;

            f(m_glfwMonitor, &x, &y);

            return {x, y};
        }

    private:
        GLFWmonitor* m_glfwMonitor;
        void* m_nativeHandle;

        bool m_isActive{true};
        std::string m_Name;
};

}  // namespace Schism::Core
