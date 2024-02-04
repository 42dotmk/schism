#include "Chess.h"

#include <utility>


#include "imgui.h"
#include "schism/Components/Sprite.h"
#include "schism/Renderer/SpriteRenderer.h"
#include "Server/Messages.h"
#include "Common.h"

using namespace Schism;

namespace Chess
{
	Chess::Chess(Core::SharedContextRef ctx, const std::string& name)
		:
		IScene(std::move(ctx), name),
		m_Camera(0, m_Ctx->window->GetWidth(), m_Ctx->window->GetHeight(), 0)
	{
		BoardRenderer::Resources sprites;

		sprites.board.sprite = m_Assets.Textures.Load("board", "res/chess/board_alt.png", true);
		sprites.validMove.sprite = m_Assets.Textures.Load("validMove", "res/chess/valid_move.png", true);
		sprites.whiteSprites[PieceType_Pawn].sprite = m_Assets.Textures.Load("WPawn", "res/chess/white_pawn.png", true);
		sprites.whiteSprites[PieceType_Knight].sprite = m_Assets.Textures.Load("WLeftKnight", "res/chess/white_knight.png", true);
		sprites.whiteSprites[PieceType_Bishop].sprite = m_Assets.Textures.Load("WLeftBishop", "res/chess/white_bishop.png", true);
		sprites.whiteSprites[PieceType_Queen].sprite = m_Assets.Textures.Load("WQueen", "res/chess/white_queen.png", true);
		sprites.whiteSprites[PieceType_Rook].sprite = m_Assets.Textures.Load("WRook", "res/chess/white_rook.png", true);
		sprites.whiteSprites[PieceType_King].sprite = m_Assets.Textures.Load("WKing", "res/chess/white_king.png", true);
		
		sprites.blackSprites[PieceType_Pawn].sprite = m_Assets.Textures.Load("BPawn", "res/chess/black_pawn.png", true);
		sprites.blackSprites[PieceType_Knight].sprite = m_Assets.Textures.Load("BLeftKnight", "res/chess/black_knight.png", true);
		sprites.blackSprites[PieceType_Bishop].sprite = m_Assets.Textures.Load("BLeftBishop", "res/chess/black_bishop.png", true);
		sprites.blackSprites[PieceType_Queen].sprite = m_Assets.Textures.Load("BQueen", "res/chess/black_queen.png", true);
		sprites.blackSprites[PieceType_Rook].sprite = m_Assets.Textures.Load("BRook", "res/chess/black_rook.png", true);
		sprites.blackSprites[PieceType_King].sprite = m_Assets.Textures.Load("BKing", "res/chess/black_king.png", true);

		int width = m_Ctx->window->GetWidth();
		int height = m_Ctx->window->GetHeight();
		float offsetPercentage = 0.32;
		float pieceSize = (width - (width * offsetPercentage)) / 8;
		m_BoardRenderer.Init(std::move(sprites), width, height, pieceSize, 0.16f);

        m_Game = std::make_shared<Game>(m_BoardRenderer, m_NetworkSendBus);
        std::string host = "localhost"; // Temporary
        std::string port = "6666";
        m_GameClient = std::make_shared<GameClient>(host, port, m_NetworkReceiveBus);

        m_NetworkSendBus.AttachListener(m_GameClient);
        m_NetworkReceiveBus.AttachListener(m_Game);
	}

	Chess::~Chess() = default;

	void Chess::OnAttach()
	{
        m_GameClientThread = std::jthread([this]()
                                          {
                                                m_GameClient->Start();
                                          });
	}

	void Chess::OnDetach()
	{
        m_GameClient->Stop();
	}

	void Chess::OnPause()
	{

	}

	void Chess::OnResume()
	{

	}

	void Chess::OnSystemEvent(Event& e)
	{
		m_Game->ProcessInput(e);
	}

	void Chess::OnUpdate(Timestep ts)
	{
        m_Game->Update();
        m_GameClient->PollEvents();
	}

	void Chess::OnDraw()
	{
     

		SpriteRenderer::BeginScene(m_Camera.GetProjectionMatrix());
		m_Game->DrawBoard();

		ImGui::Begin("Options");
        if (ImGui::Button("Start game"))
        {
            Net::RequestGame game{};
            m_NetworkSendBus.PostEvent<Net::RequestGame>(game);
        }
        if (ImGui::Button("Undo Move"))
        {
            m_Game->UndoMove();
        }
		ImGui::End();
    }
}
