#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "schism/Scripting/Lua/Binding.h"

namespace Schism::Components
{
	struct Transform2D: public Scripting::Lua::Binding
	{
		glm::vec3 position;
		glm::vec2 scale;
		float rotation;
        
        Transform2D() = default;
        Transform2D(const glm::vec3& _position, const glm::vec2& _scale, float _rotation)
        {
            position = _position;
            scale = _scale;
            rotation = _rotation;
        }

        void Bind(sol::state& lua)
        {
            lua.new_usertype<Transform2D>(
                    "Transform2D",
                    sol::constructors<Transform2D(), Transform2D(glm::vec3, glm::vec2, float)>(),
                    "position", &Transform2D::position,
                    "scale", &Transform2D::scale,
                    "rotation", &Transform2D::rotation
                    );
        }
	};
}
