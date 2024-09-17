#include "RenderAPI.h"

#include "glad/glad.h"

#include <bgfx/bgfx.h>
#include <bgfx/defines.h>
#include <bgfx/platform.h>

namespace Schism::Renderer {
bool API::Init(Core::WindowRef window) {

    // Renderer2D::Init();
    // This will be deprecated, stays here to test other parts while the batch renderer gets finished

    bgfx::Init bgfxInit;

#if defined(SCHISM_PLATFORM_WINDOWS) || defined(SCHISM_PLATFORM_LINUX)
    bgfxInit.type = bgfx::RendererType::Vulkan;
#elif defined(SCHISM_PLATFORM_MAC)
    bgfxInit.type = bgfx::RendererType::Metal;
#else
    SC_STATIC_FAIL("Currently we are not supporting this platform");
#endif

    bgfxInit.resolution.width = window->GetWidth();
    bgfxInit.resolution.height = window->GetHeight();

    /*bgfxInit.resolution.reset = BGFX_RESET_VSYNC;*/

    bgfxInit.platformData.nwh = window->GetNativeHandle();
    bgfxInit.platformData.ndt = window->GetNativeDisplay();

    int k = 1;

    bgfx::renderFrame();
    if (!bgfx::init(bgfxInit)) {
        return false;
    }

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(0, 0, 0, window->GetWidth(), window->GetHeight());

    return true;
}

void API::Shutdown() {
    // Renderer2D::Shutdown();
}

void API::SetClearColor(const glm::vec4& color) {}

void API::Clear() {}

void API::SetViewport(uint32_t width, uint32_t height) {}
}  // namespace Schism::Renderer
