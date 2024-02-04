#pragma once
#include <cstdint>

#include <glm/vec4.hpp>

namespace Schism::Renderer
{
	class API
	{
	public:
		static void Init();
		static void Shutdown();

		static void SetClearColor(const glm::vec4& color);
		static void SetViewport(uint32_t width, uint32_t height);
		static void Clear();
	};
}
