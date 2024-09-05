#include "Sprite.h"

#include <entt/entt.hpp>
#include "schism/Components/Sprite.h"
#include "schism/Components/Transform2D.h"

namespace Schism {
Sprite::Sprite(entt::entity id, entt::registry& reg)
    : m_Id(id),
      m_Reg(reg),
      m_Transform(reg.emplace<Components::Transform2D>(id)) {}

void Sprite::SetTexture(Resources::TextureHandle handle) {}

void Sprite::SetPosition(float x, float y) {}

void Sprite::SetScale(float x, float y) {}
}  // namespace Schism