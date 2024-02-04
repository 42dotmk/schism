#pragma once

#include "schism/Core/Resources.h"
#include <cstdint>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Schism
{
    struct FontCharacter
    {
        std::uint32_t char_index;
        std::int32_t width;
        std::int32_t height;
        std::uint32_t x;
        std::uint32_t y;
    };

    struct Font
    {
        FT_Face face;
        uint8_t pointSize;
        std::vector<FontCharacter> characters;

        std::uint32_t atlas_width;
        std::uint32_t atlas_height;
        Ref<Renderer::Texture> atlas;
        uint8_t atlas_texture_index;
    };

    class TextRenderer
    {
    public:
        static void Init();
        static void Deinit();
        static Font LoadFontFace(const std::string &fontPath);
        static void RenderFontToAtlas(Font &font, const Ref<Renderer::Texture>& atlas);

    private:
        // Text renderer internal state
        static FT_Library m_library;
    };

} // Schism
