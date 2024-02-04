#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Schism::Renderer
{
	Texture::Texture()
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

    Texture::Texture(std::int32_t width, std::int32_t height, std::int8_t num_channels, GLenum format)
        : Texture()
    {
        m_Width = width;
        m_Height = height;
        m_ChannelCount = num_channels;
        m_Format = format;

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, m_Format, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, nullptr);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

	Texture::Texture(const std::string& path, bool pixelart)
	{
		//stbi_set_flip_vertically_on_load(1);
		auto data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_ChannelCount, 0);

		if (!data)
		{
			SC_CORE_CRITICAL("No texture data found :{0}", path);
			SC_CORE_TRACE("No texture data found");
			return;
		}

		if (m_ChannelCount == 3)
		{
			m_Format = GL_RGB;
		}
		if (m_ChannelCount == 4)
		{
			m_Format = GL_RGBA;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		GLint filtering = GL_LINEAR;

		if (pixelart)
		{
			filtering = GL_NEAREST;
		}

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, filtering);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, filtering);

		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Can be done with glTextureSubImage2D
		//glTextureSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_2D, 0, m_Format, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	Ref<Texture> Texture::CreateRef(const std::string& path, bool pixelart)
	{
		return MakeRef<Texture>(path, pixelart);
	}

    Ref<Texture> Texture::CreateRef(std::int32_t width, std::int32_t height, std::int8_t num_channels, GLenum format)
    {
        return MakeRef<Texture>(width, height, num_channels, format);
    }

	void Texture::SetData(uint8_t* data, uint32_t size)
	{
		//TODO: Add size checks to prevent overflow with the width and height
		glTexImage2D(GL_TEXTURE_2D, 0, m_Format, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, data);
	}

    void Texture::SetSubData(uint8_t* data, uint32_t x_offset, uint32_t y_offset, uint32_t width, uint32_t height)
    {
        glTextureSubImage2D(m_TextureID, 0, x_offset, y_offset, width, height, m_Format, GL_UNSIGNED_BYTE, data);
    }

	void Texture::Bind(uint8_t slot)
	{
		glActiveTexture(GetTexSlot(slot));
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_TextureID);
	}
}