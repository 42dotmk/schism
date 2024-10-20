#pragma once

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <schism/System/Debug.h>
#include <schism/Core/Window.h>

namespace Schism::Core::detail {

inline static void* glfwNativeWindowHandler(Window& window) {
#if defined(SCHISM_PLATFORM_LINUX)

#if defined(SCHISM_LINUX_WAYLAND)

#else
    return
#endif

#elif defined(SCHISM_PLATFORM_WINDOWS)
    return glfwGetWin32Window(window);
#elfi defined(SCHISM_PLATFORM_MAC)
    return glfwGetCocoaWindow(window);
#else
    SC_ASSERT(false, "This platform is not yet supported");

    return nullptr;
#endif
};
}  // namespace Schism::Core::detail
