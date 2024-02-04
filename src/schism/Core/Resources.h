#pragma once
#include "entt/resource/resource.hpp"
#include "schism/Gl/Shader.h"
#include "schism/Renderer/Texture.h"

namespace Schism::Resources
{
	using ShaderHandle = entt::resource<Gl::Shader>;
	using TextureHandle = entt::resource<Renderer::Texture>;
}
