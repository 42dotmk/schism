#pragma once

#include <entt/entt.hpp>

namespace Schism::Core
{
	template<typename ResourceType, typename ResourceLoader>
	class AssetManager
	{
	public:
		AssetManager()
		{
			m_Resource = {};
		}
		
		template<typename ...Args>
		entt::resource<ResourceType> Load(const char* identifier, Args&& ...args)
		{
            auto ret = m_Resource.load(entt::hashed_string{ identifier }, std::forward<Args>(args)...);

            if (ret.second)
            {
                return ret.first->second;
            }
			return {};
		}

		entt::resource<ResourceType> GetHandle(const char* identifier)
		{
			return m_Resource[entt::hashed_string{ identifier }];
		}
	private:
		entt::resource_cache<ResourceType, ResourceLoader> m_Resource;
	};
}