#include "IScene.h"

#include <schism/Game/Entity.h>

namespace Schism {
IScene::IScene(Core::SharedContextRef ctx, const std::string& name)
    : m_Ctx(ctx), m_Name(name) {}

Entity IScene::CreateEntity() {
    return {this, m_Registry.create()};
}

IScene::~IScene() {}
}  // namespace Schism
