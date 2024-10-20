#include "RenderAPI.h"

#include "glad/glad.h"

#include <bgfx/bgfx.h>
#include <bgfx/defines.h>
#include <bgfx/platform.h>
#include <optional>

namespace Schism::Renderer {
std::optional<bgfx::RendererType::Enum> API::Init(Core::WindowRef window) {
    bgfx::Init bgfxInit;

#if defined(SCHISM_PLATFORM_WINDOWS)
    bgfxInit.type = bgfx::RendererType::Direct3D12;
#elif defined(SCHISM_PLATFORM_MAC)
    bgfxInit.type = bgfx::RendererType::Metal;
#elif defined(SCHISM_PLATFORM_LINUX)
    bgfxInit.type = bgfx::RendererType::Vulkan;
#else
    SC_STATIC_FAIL("Currently we are not supporting this platform");
#endif

    bgfxInit.resolution.width = window->GetWidth();
    bgfxInit.resolution.height = window->GetHeight();

    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;

    bgfxInit.platformData.nwh = window->GetNativeHandle();
    bgfxInit.platformData.ndt = window->GetNativeDisplay();

    // here we would start the render thread on the main thread
    // call to renderFrame so bgfx doesn't start a render trhead
    // but in the future this should be defered into an another thread
    bgfx::renderFrame();
    if (!bgfx::init(bgfxInit)) {
        return std::nullopt;
    }

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
    bgfx::setViewRect(0, 0, 0, window->GetWidth(), window->GetHeight());

    // here we would instantiate all of the specific renderers
    return bgfxInit.type;
}

void API::Shutdown() {
    // Renderer2D::Shutdown();
}

void API::SetClearColor(const glm::vec4& color) {}

void API::Clear() {}

void API::SetViewport(uint32_t width, uint32_t height) {
    bgfx::setViewRect(0, 0, 0, width, height);
}
}  // namespace Schism::Renderer
