#include "Renderer2D.h"

#include <glm/ext/matrix_transform.hpp>

#include <parallel_hashmap/phmap.h>
#include "schism/Core/ResourceGlobals.h"
#include "schism/Gl/VertexArray.h"
#include "schism/Gl/VertexBuffer.h"

namespace Schism
{
	using TextureSlotType = uint8_t;
	
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		int TexIndex;
	};

	struct Render2DData
	{
		static constexpr uint32_t MaxQuads = 10000;
		static constexpr uint32_t MaxVertices = MaxQuads * 4;
		static constexpr uint32_t MaxIndices = MaxQuads * 6;
		static constexpr uint32_t MaxTextureSlots = 32;
		
		Ref<Gl::VertexArray> VertArray;
		Ref<Gl::VertexBuffer> VertBuff;
		Ref<Gl::IndexBuffer> IndexBuff;
		Ref<Gl::Shader> Shader;
		// Add default texture

		uint32_t QuadIndexCount = 0;
		uint32_t VertexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
		
		glm::vec4 QuadVertexPositions[4] = 
		{
			{ -1.f, 1.f, 0.f, 1.f },
			{ -1.f, -1.f, 0.f, 1.f },
			{ 1.f, -1.f, 0.f, 1.f },
			{ 1.f, 1.f, 0.f, 1.f }
		};

		phmap::flat_hash_map<TextureSlotType, Resources::TextureHandle> Textures;
		TextureSlotType TextureSlotCount = 0;
	};

	static Render2DData s_RenderData;
	
	void Renderer2D::Init()
	{
		s_RenderData.VertArray = Gl::VertexArray::Create();
		s_RenderData.VertBuff = Gl::VertexBuffer::Create({ {
			{ Gl::ShaderDataType::Float3, "a_Position " },
			{ Gl::ShaderDataType::Float4, "a_Color" },
			{ Gl::ShaderDataType::Float2, "a_TexCoord" },
			{ Gl::ShaderDataType::Int, "a_Tex" }
		} });
		s_RenderData.IndexBuff = Gl::IndexBuffer::Create();
		s_RenderData.Shader = Gl::Shader::Create(
			Resources::Engine::Renderer2D.vert,
			Resources::Engine::Renderer2D.frag
		);

		s_RenderData.VertArray->AddVertexBuffer(s_RenderData.VertBuff);
		s_RenderData.VertArray->SetIndexBuffer(s_RenderData.IndexBuff);
		
		s_RenderData.QuadVertexBufferBase = new QuadVertex[Render2DData::MaxVertices];
		s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;
		uint32_t* indices = new uint32_t[Render2DData::MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < Render2DData::MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}
		s_RenderData.IndexBuff->SetData(indices, Render2DData::MaxIndices);

		delete[] indices;
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_RenderData.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const ICamera& camera)
	{
		const auto& vp = camera.GetViewProjectionMatrix();

		s_RenderData.Shader->Bind();
		s_RenderData.Shader->SetMat4("u_ViewProjection", vp);

		BeginBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::DrawQuad(
		const glm::vec2& position, 
		const glm::vec2& size, 
		Resources::TextureHandle texture, 
		const glm::vec4& tintColor
	)
	{
		DrawQuad(glm::vec3{ position.x, position.y, 0.f }, size, texture, tintColor);
	}

	void Renderer2D::DrawQuad(
		const glm::vec3& position, 
		const glm::vec2& size, 
		Resources::TextureHandle texture, 
		const glm::vec4& tintColor
	)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), position)
			* glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		
		DrawQuad(transform, texture, tintColor);
	}

	void Renderer2D::DrawQuad(
		const glm::vec2& position, 
		const glm::vec2& size, 
		Resources::TextureHandle texture, 
		const std::array<float, 8>& texCoords, 
		const glm::vec4& tintColor
	)
	{
		DrawQuad(glm::vec3{ position.x, position.y, 0.f }, size, texture, texCoords, tintColor);
	}

	void Renderer2D::DrawQuad(
		const glm::vec3& position, 
		const glm::vec2& size, 
		Resources::TextureHandle texture, 
		const std::array<float, 8>& texCoords, 
		const glm::vec4& tintColor
	)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), position)
			* glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });

		DrawQuad(transform, texture, texCoords, tintColor);
	}

	void Renderer2D::DrawQuad(
		const glm::mat4& transform, 
		Resources::TextureHandle texture, 
		const glm::vec4& tintColor)
	{
		static constexpr std::array<float, 8> texCoords = {
			0.f, 1.f,
			0.f, 0.f,
			1.f, 0.f,
			1.f, 1.f
		};

		DrawQuad(transform, texture, texCoords, tintColor);
	}

	void Renderer2D::DrawQuad(
		const glm::mat4& transform, 
		Resources::TextureHandle texture,
		const std::array<float, 8>& texCoord, 
		const glm::vec4& tintColor
	)
	{
		if (
			s_RenderData.VertexCount > Render2DData::MaxVertices||
			s_RenderData.TextureSlotCount >= Render2DData::MaxTextureSlots)
		{
			NextBatch();
		}

		// TODO: Find a way to optimize this
		// Array would be better for data locality and to avoid cache misses
		// but a hash map would be better for using smaller amounts of textures
		// so that needs to be tested in both cases  
		int texIndex = -1;
		for (auto& [slot, savedTexture] : s_RenderData.Textures)
		{
			/* Temporary commented, this should be reworked!
			if (*savedTexture == *texture)
			{
				texIndex = slot;
				break;
			}
			*/
		}
		if (texIndex == -1)
		{
			s_RenderData.Textures[s_RenderData.TextureSlotCount] = texture;
			texIndex = s_RenderData.TextureSlotCount;
			s_RenderData.TextureSlotCount++;
		}
		
		for (int i = 0; i < 4; i++)
		{
			s_RenderData.QuadVertexBufferPtr->Color = tintColor;
			s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[i];
			s_RenderData.QuadVertexBufferPtr->TexCoord = { texCoord[i * 2], texCoord[i * 2 + 1] };
			s_RenderData.QuadVertexBufferPtr->TexIndex = texIndex;
			s_RenderData.QuadVertexBufferPtr++;
			s_RenderData.VertexCount++;
		}

		s_RenderData.QuadIndexCount += 6;
	}

	void Renderer2D::Flush()
	{
		for (auto& [slot, texture] : s_RenderData.Textures)
		{
			texture->Bind(slot);
		}

		s_RenderData.VertBuff->SetData(s_RenderData.QuadVertexBufferBase, sizeof(uint32_t) * s_RenderData.VertexCount);
		s_RenderData.VertArray->Bind();
		
		glDrawElements(GL_TRIANGLES, s_RenderData.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::BeginBatch()
	{
		s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;
		s_RenderData.QuadIndexCount = 0;
		s_RenderData.VertexCount = 0;
		s_RenderData.Textures.clear();
		s_RenderData.TextureSlotCount = 0;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		BeginBatch();
	}
}
