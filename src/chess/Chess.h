#include "schism/Interfaces/IScene.h"
#include "schism/Renderer/OrthographicCamera.h"
#include "schism/Core/Assets.h"
#include "BoardRenderer.h"
#include "Engine.h"
#include "Game.h"
#include "GameClient.h"
#include "schism/Game/GameEvent/CallbackBus.h"
#include "schism/Game/GameEvent/SyncBus.h"
#include "schism/Game/GameEvent/SyncListener.h"

using namespace Schism;

namespace Chess
{
	class Chess : public IScene
	{
	public:
		Chess(Core::SharedContextRef ctx, const std::string& name);
		~Chess();

		void OnAttach() override;
		void OnDetach() override;
		void OnPause() override;
		void OnResume() override;
		void OnUpdate(Timestep ts) override;
		void OnDraw() override;
		void OnSystemEvent(Event& e) override;
	private:
        Schism::GameEvent::CallbackBus m_NetworkSendBus;
        Schism::GameEvent::CallbackBus m_NetworkReceiveBus;
        std::jthread m_GameClientThread;
		Core::Assets m_Assets;
		BoardRenderer m_BoardRenderer;
		std::shared_ptr<Game> m_Game;
        std::shared_ptr<GameClient> m_GameClient;
		Renderer::OrthographicCamera m_Camera;
	};
}
