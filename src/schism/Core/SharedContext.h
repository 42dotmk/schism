#pragma once

#include "Assets.h"
#include "Window.h"
#include "schism/System/Ptr.h"

namespace Schism::Core {
struct SceneManagerCallbacks {
        std::function<void(const std::string&)> Switch{nullptr};
        std::function<void(const std::string&)> Destroy{nullptr};
};

struct SharedContext {
        Ref<Window> window;
        Assets GlobalAssets;
        SceneManagerCallbacks SceneManager;
};

using SharedContextRef = Ref<SharedContext>;

static const SharedContextRef CreateSharedContext(Ref<Window> window) {
    Ref<SharedContext> Ctx = MakeRef<SharedContext>();

    Ctx->window = window;
    return Ctx;
}

}  // namespace Schism::Core
