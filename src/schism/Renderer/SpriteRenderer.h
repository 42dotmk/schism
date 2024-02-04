#pragma once

#include "schism/Gl/VertexArray.h"
#include "schism/Gl/VertexBuffer.h"
#include "schism/Core/Resources.h"

#include "schism/Components/Transform2D.h"
#include "schism/Components/Sprite.h"

namespace Schism
{
	class SpriteRenderer
	{
	public:
		static void Init();
		static void Init(Resources::ShaderHandle shader);
		static void Shutdown();
		
		static void RegisterShader(Resources::ShaderHandle shader);

		static void BeginScene(const glm::mat4& proj);

		static void Draw(Components::Transform2D& transform, Components::Sprite& sprite, const glm::mat4& proj);
		static void Draw(Components::Transform2D& transform, Components::Sprite& sprite);
	private:
		static Ref<Gl::VertexBuffer> s_VertBuff;
		static Ref<Gl::VertexArray> s_VertexArray; 
		static Resources::ShaderHandle s_Shader;
		static glm::mat4 m_Projection;
		static constexpr float vertices[] = {
			0.0f, 1.0f,		0.0f, 1.0f,
			1.0f, 0.0f,		1.0f, 0.0f,
			0.0f, 0.0f,		0.0f, 0.0f,

			0.0f, 1.0f,		0.0f, 1.0f,
			1.0f, 1.0f,		1.0f, 1.0f,
			1.0f, 0.0f,		1.0f, 0.0f
		};
	};
}
