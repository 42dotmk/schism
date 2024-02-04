#pragma once
#include <string>
#include <array>

#include "Texture.h"
#include "schism/Core/Resources.h"

namespace Schism::Renderer
{
	class TextureAtlas
	{
	public:
		TextureAtlas();
		TextureAtlas(Resources::TextureHandle textureHandle);
		void SetInstanceSize(const glm::vec2& size);
		void Bind(uint8_t slot);
		std::array<float, 8> GetTextureCord(int x, int y);
	private:
		Resources::TextureHandle m_Texture;
		// Width and height of the texture in pixels
		int m_Width{ 0 };
		int m_Height{ 0 };
		glm::vec2 m_InstSize{ 0, 0 };
	};
}
