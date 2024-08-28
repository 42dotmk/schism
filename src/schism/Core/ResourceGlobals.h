#pragma once

namespace Schism::Resources {
struct ShaderPaths {
        const char* vert;
        const char* frag;
};

struct Engine {
        static constexpr inline ShaderPaths Renderer2D{
            "res/shaders/renderer2d.vert", "res/shaders/renderer2d.frag"};
};

}  // namespace Schism::Resources
