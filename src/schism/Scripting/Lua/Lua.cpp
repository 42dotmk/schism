#include "Lua.h"

#include "Bindings/Glm.h"

namespace Schism::Scripting::Lua
{
    LuaState::LuaState()
    {
        m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);

        GlmBindings glm;
        RegisterBinding<GlmBindings>();
    }
}
