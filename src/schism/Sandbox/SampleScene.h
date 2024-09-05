#include "schism/Interfaces/IScene.h"
#include "schism/Renderer/OrthographicCamera.h"
#include "schism/Renderer/SpriteRenderer.h"

using namespace Schism;

class SampleScene : public IScene {
    public:
        SampleScene(Core::SharedContextRef ctx, const std::string& name);
        ~SampleScene();

        void OnAttach() override;
        void OnDetach() override;
        void OnPause() override;
        void OnResume() override;
        void OnUpdate(Timestep ts) override;
        void OnDraw() override;
        void OnSystemEvent(Event& e) override;

    private:
        entt::entity m_Ship1;
        entt::registry m_Registry;
        SpriteRenderer m_Renderer;
        Renderer::OrthographicCamera m_Camera;
};