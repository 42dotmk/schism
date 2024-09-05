#pragma once
#include "Texture.h"

#include "entt/entity/fwd.hpp"
#include "schism/Components/Transform2D.h"
#include "schism/Core/Resources.h"

namespace Schism {
// Temporary
class Sprite {
    public:
        Sprite(entt::entity id, entt::registry& registry);
        void SetTexture(Resources::TextureHandle handle);
        void SetPosition(float x, float y);
        void SetScale(float w, float h);

    private:
        entt::entity m_Id;
        entt::registry& m_Reg;
        Components::Transform2D& m_Transform;
};
}  // namespace Schism
