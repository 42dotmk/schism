#include <GLFW/glfw3.h>

#include <schism/Core/Display.h>
#include <glm/glm.hpp>

namespace Schism::Core {
Display::Display(GLFWmonitor* monitor)
    : m_glfwMonitor(monitor), m_Name(glfwGetMonitorName(monitor)) {}

void* Display::getNativeHandle() {}

std::string Display::Name() {
    return m_Name;
}

glm::vec2 Display::PhysicalSize() {
    return vec2Getter<int>(&glfwGetMonitorPhysicalSize);
}

glm::vec2 Display::ContentScale() {
    return vec2Getter<float>(&glfwGetMonitorContentScale);
}

glm::vec2 Display::VirtualPosition() {
    return vec2Getter<int>(&glfwGetMonitorPos);
}

glm::vec4 Display::WorkArea() {
    int x;
    int y;

    int w;
    int h;

    glfwGetMonitorWorkarea(m_glfwMonitor, &x, &y, &w, &h);

    return {x, y, w, h};
}

std::vector<struct Display::VideoMode> Display::VideoModes() {}

struct Display::VideoMode Display::VideoMode() {}
}  // namespace Schism::Core
