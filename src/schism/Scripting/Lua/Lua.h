#pragma once


#include "Binding.h"

#include <sol/sol.hpp>

namespace Schism::Scripting::Lua
{
    class LuaState
    {
    public:
        LuaState();

        template<typename Bindable, typename ...Args>
        void RegisterBinding(Args&& ...args)
        {
            // this works because the template will create multiple funciton bodies
            static bool isBounded = false;

            if (isBounded)
            {
                return;
            }

            Bindable::Bind(m_lua, std::forward<Args...>(args)...);
            isBounded = true;
        }

        // ONLY TEMPORARY, SUBJECT TO CHANGE
        bool LoadScriptFile(const std::string& filepath)
        {
            auto k = m_lua.script_file(filepath);

            return true;
        }
    private:
        sol::state m_lua;
    };
}
