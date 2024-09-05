#include "TextureAtlas.h"

namespace Schism::Renderer {
TextureAtlas::TextureAtlas() {}

TextureAtlas::TextureAtlas(Resources::TextureHandle textureHandle) {
    m_Texture = textureHandle;
    m_Width = m_Texture->GetWidth();
    m_Height = m_Texture->GetHeight();
}

void TextureAtlas::Bind(uint8_t slot) {
    m_Texture->Bind(slot);
}

void TextureAtlas::SetInstanceSize(const glm::vec2& size) {
    m_InstSize = size;
}

std::array<float, 8> TextureAtlas::GetTextureCord(int x, int y) {
    float xStart = m_InstSize.x * x;
    float xEnd = xStart + m_InstSize.x;
    float yStart = m_Height - (m_InstSize.y * (y + 1));
    float yEnd = m_Height - (m_InstSize.y * y);

    float xStartNormalized = xStart / m_Width;
    float xEndNormalized = xEnd / m_Width;
    float yStartNormalized = yStart / m_Height;
    float yEndNormalized = yEnd / m_Height;

    return {xStartNormalized, yStartNormalized, xStartNormalized,
            yEndNormalized,   xEndNormalized,   yEndNormalized,
            xEndNormalized,   yStartNormalized};
}

}  // namespace Schism::Renderer
