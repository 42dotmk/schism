#pragma once
#include <sol/overload.hpp>
#include <sol/raii.hpp>
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <sol/types.hpp>

#include "schism/Scripting/Lua/Binding.h"

namespace Schism::Scripting::Lua
{
    class GlmBindings: public Binding 
    {
    public:
        void Bind(sol::state& lua) override
        {
            registerVec2(lua);
            registerVec3(lua);
            registerVec4(lua);
        }
    private:
        void registerVec2(sol::state& lua) 
        {
            auto addition = sol::overload(
                    [](const glm::vec2& vec, const glm::vec2& vec2) { return vec + vec2; },
                    [](const glm::vec2& vec, float v) { return vec + v; },
                    [](float v, const glm::vec2& vec) { return v + vec; }
                    );

            auto subtract = sol::overload(
                    [](const glm::vec2& vec, const glm::vec2& vec2) { return vec - vec2; },
                    [](const glm::vec2& vec, float v) { return vec - v; },
                    [](float v, const glm::vec2& vec) { return v - vec; }
                    );

            auto multiply = sol::overload(
                    [](const glm::vec2& vec, const glm::vec2& vec2) { return vec * vec2; },
                    [](const glm::vec2& vec, float v) { return vec * v; },
                    [](float v, const glm::vec2& vec) { return v * vec; }
                    );

            
            auto divide = sol::overload(
                    [](const glm::vec2& vec, const glm::vec2& vec2) { return vec / vec2; },
                    [](const glm::vec2& vec, float v) { return vec / v; },
                    [](float v, const glm::vec2& vec) { return v / vec; }
                    );
            
            lua.new_usertype<glm::vec2>(
                    "vec2",
                    sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
                    "x", &glm::vec2::x,
                    "y", &glm::vec2::y,
                    sol::meta_function::multiplication, multiply,
                    sol::meta_function::addition, addition,
                    sol::meta_function::division, divide,
                    sol::meta_function::subtraction, subtract
                    );
        }

        void registerVec3(sol::state& lua)
        {
            auto addition = sol::overload(
                    [](const glm::vec3& vec, const glm::vec3& vec2) { return vec + vec2; },
                    [](const glm::vec3& vec, float v) { return vec + v; },
                    [](float v, const glm::vec3& vec) { return v + vec; }
                    );

            auto subtract = sol::overload(
                    [](const glm::vec3& vec, const glm::vec3& vec2) { return vec - vec2; },
                    [](const glm::vec3& vec, float v) { return vec - v; },
                    [](float v, const glm::vec3& vec) { return v - vec; }
                    );

            auto multiply = sol::overload(
                    [](const glm::vec3& vec, const glm::vec3& vec2) { return vec * vec2; },
                    [](const glm::vec3& vec, float v) { return vec * v; },
                    [](float v, const glm::vec3& vec) { return v * vec; }
                    );

            
            auto divide = sol::overload(
                    [](const glm::vec3& vec, const glm::vec3& vec2) { return vec / vec2; },
                    [](const glm::vec3& vec, float v) { return vec / v; },
                    [](float v, const glm::vec3& vec) { return v / vec; }
                    );
            
            lua.new_usertype<glm::vec3>(
                    "vec3",
                    sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
                    "x", &glm::vec3::x,
                    "y", &glm::vec3::y,
                    "z", &glm::vec3::z,
                    sol::meta_function::multiplication, multiply,
                    sol::meta_function::addition, addition,
                    sol::meta_function::division, divide,
                    sol::meta_function::subtraction, subtract
                    );
        }

        void registerVec4(sol::state& lua)
        {
            auto addition = sol::overload(
                    [](const glm::vec4& vec, const glm::vec4& vec2) { return vec + vec2; },
                    [](const glm::vec4& vec, float v) { return vec + v; },
                    [](float v, const glm::vec4& vec) { return v + vec; }
                    );

            auto subtract = sol::overload(
                    [](const glm::vec4& vec, const glm::vec4 vec2) { return vec - vec2; },
                    [](const glm::vec4& vec, float v) { return vec - v; },
                    [](float v, const glm::vec4& vec) { return v - vec; }
                    );

            auto multiply = sol::overload(
                    [](const glm::vec4& vec, const glm::vec4& vec2) { return vec * vec2; },
                    [](const glm::vec4& vec, float v) { return vec * v; },
                    [](float v, const glm::vec4& vec) { return v * vec; }
                    );

            
            auto divide = sol::overload(
                    [](const glm::vec4& vec, const glm::vec4& vec2) { return vec / vec2; },
                    [](const glm::vec4& vec, float v) { return vec / v; },
                    [](float v, const glm::vec4& vec) { return v / vec; }
                    );
            
            lua.new_usertype<glm::vec4>(
                    "vec4",
                    sol::constructors<glm::vec4(), glm::vec4(float), glm::vec4(float, float, float, float)>(),
                    "x", &glm::vec4::x,
                    "y", &glm::vec4::y,
                    "z", &glm::vec4::z,
                    "w", &glm::vec4::w,
                    sol::meta_function::multiplication, multiply,
                    sol::meta_function::addition, addition,
                    sol::meta_function::division, divide,
                    sol::meta_function::subtraction, subtract
                    );
        }
    };
}
