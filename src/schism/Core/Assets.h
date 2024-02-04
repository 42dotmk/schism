#pragma once

#include <entt/entt.hpp>

#include "AssetManager.h"
#include "schism/Audio/Source.h"
#include "schism/Gl/Shader.h"
#include "schism/Renderer/Texture.h"
#include "schism/System/FileIO.h"

namespace Schism::Core
{
	struct TextureLoader
	{
        using result_type = Ref<Renderer::Texture>;

		result_type operator()(const std::string& path, bool pixelart = false) const
		{
			return Renderer::Texture::CreateRef(path, pixelart);
		}
	};

	struct ShaderLoader
	{
        using result_type = Ref<Gl::Shader>;

        result_type operator()(const std::string& vertPath, const std::string& fragPath)
        {
			return Gl::Shader::Create(vertPath, fragPath);
		}
	};

    struct AudioLoader
    {
        using result_type = Ref<Audio::Source>;

        result_type operator()(const std::string& filepath)
        {
            return Audio::Source::Create(filepath);
        }
    };

	struct Assets
	{
		AssetManager<Renderer::Texture, TextureLoader> Textures;
		AssetManager<Gl::Shader, ShaderLoader> Shaders;
        AssetManager<Audio::Source, AudioLoader> Audio; 
	};
	
}
