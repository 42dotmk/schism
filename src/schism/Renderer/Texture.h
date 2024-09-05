#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

#include "schism/System/System.h"

namespace Schism::Renderer {
class Texture {
    public:
        Texture();
        Texture(const std::string& path, bool pixelart = false);
        ~Texture();

        void SetData(uint8_t* data, uint32_t size);
        static Ref<Texture> CreateRef(const std::string& path,
                                      bool pixelart = false);
        void Bind(uint8_t slot);

        GLenum GetFormat() const { return m_Format; }

        int GetWidth() const { return m_Width; }

        int GetHeight() const { return m_Height; }

        const glm::vec2 GetSize() const { return {m_Width, m_Height}; }

        bool operator==(Texture& other) const {
            return other.m_TextureID == m_TextureID && other.path == path &&
                   other.m_ChannelCount == m_ChannelCount &&
                   other.m_Width == m_Width && other.m_Height == m_Height &&
                   other.m_Format == m_Format;
        }

        static int GetTexSlot(uint8_t slot) {
            static constexpr int slots[] = {
                GL_TEXTURE0,  GL_TEXTURE1,  GL_TEXTURE2,  GL_TEXTURE3,
                GL_TEXTURE4,  GL_TEXTURE5,  GL_TEXTURE6,  GL_TEXTURE7,
                GL_TEXTURE8,  GL_TEXTURE9,  GL_TEXTURE10, GL_TEXTURE11,
                GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14, GL_TEXTURE15,
                GL_TEXTURE16, GL_TEXTURE17, GL_TEXTURE18, GL_TEXTURE19,
                GL_TEXTURE20, GL_TEXTURE31, GL_TEXTURE22, GL_TEXTURE23,
                GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27,
                GL_TEXTURE28, GL_TEXTURE29, GL_TEXTURE30, GL_TEXTURE31,
            };

            SC_ASSERT(slot < 32, "Texture slot doesn't exist");

            return slots[slot];
        }

    private:
        std::string path;
        GLenum m_Format;
        unsigned m_TextureID;
        int m_Width;
        int m_Height;
        int m_ChannelCount;
};
}  // namespace Schism::Renderer
