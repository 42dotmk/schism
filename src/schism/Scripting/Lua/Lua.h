#include <sol/sol.hpp>
#include <type_traits>

#include "Binding.h"

namespace Schism::Scripting::Lua
{
    class LuaState
    {
    public:
        LuaState();

        template<typename T,
            typename = std::enable_if_t<std::is_base_of_v<Binding, T>>>
        void RegisterBinding(T& object)
        {
            object.Bind(m_lua);
        }

        // ONLY TEMPORARY, SUBJECT TO CHANGE
        bool LoadScriptFile(const std::string& filepath);
    private:
        sol::state m_lua;
    };
}
