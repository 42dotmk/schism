#pragma once

#include <glm/glm.hpp>
#include "schism/Core/Resources.h"
#include "schism/Interfaces/ICamera.h"

namespace Schism
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const ICamera& camera);
		static void EndScene();
		
		static void DrawQuad(
			const glm::vec2& position, 
			const glm::vec2& size, 
			Resources::TextureHandle texture, 
			const glm::vec4& tintColor = glm::vec4(1.f)
		);
		static void DrawQuad(
			const glm::vec3& position, 
			const glm::vec2& size, 
			Resources::TextureHandle texture, 
			const glm::vec4& tintColor = glm::vec4(1.f)
		);

		static void DrawQuad(
			const glm::vec2& position, 
			const glm::vec2& size, 
			Resources::TextureHandle texture, 
			const std::array<float, 8>& texCoords, 
			const glm::vec4& tintColor = glm::vec4(1.f)
		); 
		static void DrawQuad(
			const glm::vec3& position, 
			const glm::vec2& size, 
			Resources::TextureHandle texture, 
			const std::array<float, 8>& texCoords, 
			const glm::vec4& tintColor = glm::vec4(1.f)
		);
		
		static void DrawQuad(
			const glm::mat4& transform, 
			Resources::TextureHandle texture, 
			const glm::vec4& tintColor = glm::vec4(1.f)
		);
		static void DrawQuad(
			const glm::mat4& transform, 
			Resources::TextureHandle texture, 
			const std::array<float, 8>& texCoord, 
			const glm::vec4& tintColor = glm::vec4(1.f)
		);

	private:
		static void Flush();
		static void BeginBatch();
		static void NextBatch();
	};
}
