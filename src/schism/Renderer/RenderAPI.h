#pragma once
#include <cstdint>

#include <glm/vec4.hpp>
#include <bgfx/bgfx.h>
#include "schism/Core/Window.h"

namespace Schism::Renderer {
class API {
    public:
        static std::optional<bgfx::RendererType::Enum> Init(
            Core::WindowRef window);
        static void Shutdown();

        static void SetClearColor(const glm::vec4& color);
        static void SetViewport(uint32_t width, uint32_t height);
        static void Clear();
};
}  // namespace Schism::Renderer
