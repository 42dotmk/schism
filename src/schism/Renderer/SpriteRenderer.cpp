#include "SpriteRenderer.h"
#include "schism/System/Log.h"

#include <glm/ext/matrix_transform.hpp>

namespace Schism
{
	Ref<Gl::VertexBuffer> SpriteRenderer::s_VertBuff;
	Ref<Gl::VertexArray> SpriteRenderer::s_VertexArray;
	Resources::ShaderHandle SpriteRenderer::s_Shader;
	glm::mat4 SpriteRenderer::m_Projection;

	void SpriteRenderer::Init(Resources::ShaderHandle shader)
	{
		s_Shader = shader;

		s_VertBuff = Gl::VertexBuffer::Create({ {
			{ Gl::ShaderDataType::Float2, "position" },
			{ Gl::ShaderDataType::Float2, "texcord" },
		} });
		s_VertBuff->SetData((float*)vertices, sizeof(vertices));

		s_VertexArray = Gl::VertexArray::Create();
		s_VertexArray->AddVertexBuffer(s_VertBuff);			
	}

	void SpriteRenderer::Init()
	{
		s_VertBuff = Gl::VertexBuffer::Create({ {
			{ Gl::ShaderDataType::Float2, "position" },
			{ Gl::ShaderDataType::Float2, "texcord" },
		} });

		s_VertBuff->SetData((float*)vertices, sizeof(vertices));

		s_VertexArray = Gl::VertexArray::Create();
		s_VertexArray->AddVertexBuffer(s_VertBuff);
	}

	void SpriteRenderer::Shutdown()
	{
		// Not needed because if sprite renderer goes out of scope
		// the pointer is only used in this class so it should release it self automatically
		s_VertBuff.reset(); 
	}

	void SpriteRenderer::RegisterShader(Resources::ShaderHandle shader)
	{
		s_Shader = shader;
	}

	void SpriteRenderer::BeginScene(const glm::mat4& proj)
	{
		m_Projection = proj;
	}

	void SpriteRenderer::Draw(Components::Transform2D& transformComponent, Components::Sprite& spriteComponent, const glm::mat4& proj)
	{
        glm::mat4 transform = glm::translate(glm::mat4(1.f), transformComponent.position);
		transform = glm::translate(transform, glm::vec3(0.5f * transformComponent.scale.x, 0.5f * transformComponent.scale.y, 0.0f));
		transform = glm::rotate(transform, glm::radians(transformComponent.rotation), glm::vec3(0.f, 0.f, 1.f));
		transform = glm::translate(transform, glm::vec3(-0.5f * transformComponent.scale.x, -0.5f * transformComponent.scale.y, 0.0f));
		transform = glm::scale(transform, glm::vec3(transformComponent.scale, 1.f));

		spriteComponent.sprite->Bind(0);
	    	
		s_Shader->Bind();
		s_Shader->SetMat4("projection", proj);
		s_Shader->SetMat4("transform", transform);
		s_Shader->SetInt("sprite", 0);

		s_VertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void SpriteRenderer::Draw(Components::Transform2D& transform, Components::Sprite& sprite)
	{
		Draw(transform, sprite, m_Projection);
	}

	
}
