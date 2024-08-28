#pragma once
#include <string>

#include "schism/Core/SharedContext.h"
#include "schism/Core/Timestep.h"

namespace Schism {
class Entity;

class IScene {
    public:
        IScene(Core::SharedContextRef ctx, const std::string& name);
        virtual ~IScene();

        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void OnPause() = 0;
        virtual void OnResume() = 0;
        virtual void OnUpdate(Timestep ts) = 0;
        virtual void OnDraw() = 0;
        virtual void OnSystemEvent(Event& e) = 0;

        entt::registry& Registry() { return m_Registry; }

        Entity CreateEntity();

        const std::string& GetName() const { return m_Name; }

        bool WasPaused() const { return m_WasPaused; }

    protected:
        bool m_WasPaused{false};
        std::string m_Name;
        Core::SharedContextRef m_Ctx;
        entt::registry m_Registry;
};
}  // namespace Schism
