#include "Game.h"

#include "schism/Core/Events/MouseEvents.h"
#include "Server/Messages.h"

namespace Chess
{
	Game::Game(BoardRenderer& renderer, Schism::GameEvent::CallbackBus& networkSendBus) // Interface for the boardrenderer would be better
		:
		m_BoardRenderer{ renderer },
        m_NetworkSendBus{ networkSendBus }
	{
        ListenGameEvent<Net::GameMove>([this](Net::GameMove&& gameMove)
          {
                SC_CORE_INFO("(Chess/Game) Got move event");
				// check the game id
                // Don't accept moves if we are not waiting for a move
                // Handle when move is not validated

                if (!m_State.waitingForMove)
                {
                    SC_CORE_WARN("(Chess/Game) Got move when we were waiting for move");
                    return;
                }

                m_Engine.MakeMove(gameMove.move);
                m_State.waitingForMove = false;
          });

        ListenGameEvent<Net::EnterGame>([this](Net::EnterGame&& e)
           {
                m_State.isWhite = e.isWhite;
                m_State.waitingForMove = !e.isWhite;
                m_State.gameId = e.gameId;
                m_Engine.Reset();
           });

        ListenGameEvent<Net::SuccessfulMove>([this](Net::SuccessfulMove&& e)
         {
                if (e.gameId != m_State.gameId)
                {
                    return; // todo:: Handle multiple games
                }

                if (e.moveCount == m_State.moveCount)
                {
                    m_State.moveValidated = true;
                }
         });
	}

    void Game::UndoMove()
    {
        m_Engine.UndoLastMove();
    }


    // a bit ugly
    // TODO: refactor this
	void Game::ProcessInput(Schism::Event& e)
	{
		switch (e.GetEventType())
		{
		case Schism::MouseButtonPressedEvent::GetStaticType():
		{
			const auto& board = m_Engine.GetBoardState();
			Schism::MouseButtonPressedEvent evt = dynamic_cast<Schism::MouseButtonPressedEvent&>(e);
			auto& pos = evt.GetPosition();
			auto boardPosition = FindBoardPositionFromCoord(pos.x, pos.y);

			if (boardPosition.x > board.size() || boardPosition.y > board.size())
			{
				return;
			}

			if (!m_State.isWhite)
			{
				boardPosition = FlipBoardPosition(boardPosition);
			}

			auto selectPiece = [this](const auto& position)
			{
				if (position == m_State.selectedPosition)
				{
					return;
				}

				m_State.pieceSelected = true;
				m_State.selectedPosition = position;

				if (!m_State.isWhite)
				{
					m_ValidMoves = m_Engine.GetValidMoves(FlipBoardPosition(position));
				}
				else
				{
					m_ValidMoves = m_Engine.GetValidMoves(position);
				}
				if (!m_ValidMoves.empty())
				{
					SC_CORE_INFO("Valid move count {0}", m_ValidMoves.size());
				}
                SC_CORE_WARN("SELECTED PIECE");
			};

			if (m_State.pieceSelected)
			{
				auto& prevPiece = board[m_State.selectedPosition.x][m_State.selectedPosition.y];
				auto& currentPiece = board[boardPosition.x][boardPosition.y];

				if (prevPiece.color == currentPiece.color && IsValidPiece(currentPiece.type))
				{
					selectPiece(boardPosition);
					return;
				}

				Move m;
				m.piece = board[m_State.selectedPosition.x][m_State.selectedPosition.y];
				m.currentPosition = boardPosition;
				m.prevPosition = m_State.selectedPosition;
				m_State.pieceSelected = false;
				m_ValidMoves = {};

                if (m_State.waitingForMove)
                {
                    return;
                }

                m_Engine.MakeMove(m); // Temporary
                SC_CORE_INFO("Sending move to networkSendBus");
				Net::GameMove gameMove{ m_State.gameId, m };
                //m_NetworkSendBus.PostEvent<Net::GameMove>(gameMove);
                m_State.waitingForMove = true;
                m_State.moveValidated = false;
				return;
			}

			if (board[boardPosition.x][boardPosition.y].type == PieceType_Blank)
			{
				return;
			}

			selectPiece(boardPosition);
			
			break;
		}
		default:
			break;
		}
	}

    void Game::Update()
    {
        ProcessGameEvents();
    }

	void Game::DrawBoard()
	{
		if (!m_ValidMoves.empty())
		{
			m_BoardRenderer.DrawValidMoves(m_ValidMoves);
		}
		m_BoardRenderer.DrawBoard(m_Engine.GetBoardState(), !m_State.isWhite);
	}


}