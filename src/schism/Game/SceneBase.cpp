#include <schism/Game/SceneBase.h>

#include <entt/entity/fwd.hpp>

namespace Schism
{
	SceneBase::SceneBase(Core::SharedContextRef ctx, const std::string& name)
		:
		m_Ctx(ctx),
		m_Name(name)
	{
	}

	SceneBase::~SceneBase()
	{

	}

    entt::registry& SceneBase::Registry()
    {
        return m_Registry;
    }
}
