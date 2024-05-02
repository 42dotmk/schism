#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <sol/state.hpp>
#include <entt/entt.hpp>

namespace Schism::Components
{
	struct Transform2D 
	{
		glm::vec3 position;
		glm::vec2 scale;
		float rotation;
        static constexpr char name[] = "transform";

        Transform2D() = default;
        Transform2D(const glm::vec3& _position, const glm::vec2& _scale, float _rotation)
        {
            position = _position;
            scale = _scale;
            rotation = _rotation;
        }

        static void Bind(sol::state& lua)
        {
            lua.new_usertype<Transform2D>(
                    "Transform2D",
                    sol::constructors<Transform2D(), Transform2D(glm::vec3, glm::vec2, float)>(),
                    "type_id", &entt::type_hash<Transform2D>::value,
                    "name", sol::var(Transform2D::name),
                    "position", &Transform2D::position,
                    "scale", &Transform2D::scale,
                    "rotation", &Transform2D::rotation
                    );
        }
	};

}
