#pragma once

// This should be included when using the engine

#include "GLFW/glfw3.h"
#include "schism/Core/Application.h"
#include "schism/Renderer/Renderer2D.h"
#include "schism/Renderer/SpriteRenderer.h"
#include "schism/Sandbox/Sandbox.h"
#include "schism/System/System.h"

namespace Schism {
static void Init() {
    Log::Init();
    if (!glfwInit()) {
        SC_CORE_CRITICAL("Coudn't initialize glfw");
    }
}
}  // namespace Schism
