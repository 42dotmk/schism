
#include "Game.h"

#include <utility>
#include "Messages.h"

namespace Chess::Net
{
	Game::Game(std::weak_ptr<Client> player1, std::weak_ptr<Client> player2, bool player1Turn)
		:
		m_Player1(std::move(player1)),
		m_Player2(std::move(player2))
    {
        auto p1 = m_Player1.lock();
        auto p2 = m_Player2.lock();

        if (p1 && p2)
        {
            if (player1Turn)
            {
                m_NextTurnPlayerId = p1->Id();
            }
            else
            {
                m_NextTurnPlayerId = p2->Id();
            }
        }
        else
        {
            // log out error, invalid player
            SC_CORE_ERROR("Game got invalid player, disconnecting");
            m_NextTurnPlayerId = 0;
        }
	}

    std::shared_ptr<Game> Game::Create(std::weak_ptr<Client> p1, std::weak_ptr<Client> p2, bool player1Turn)
    {
        auto p = std::shared_ptr<Game>(new Game(std::move(p1), std::move(p2), player1Turn));
        auto id = reinterpret_cast<uint64_t>(p.get()); // very hacky way, quick fix for now
        p->m_Id = id;
        return p;
    }

	void Game::MakeMove(const Move& move, const std::weak_ptr<Client>& clientThatSentMove)
	{
        {
            auto player = clientThatSentMove.lock();

            if (!player)
            {
                // send error code
                return;
            }

            if (player->Id() != m_NextTurnPlayerId || m_NextTurnPlayerId == 0)
            {
                // send error code
                // invalid player
                return;
            }
        }

		if (!m_GameState.MakeMove(move))
		{
			SC_CORE_WARN("(Server/Game) Got invalid move");

            FailedMove failedMove{ m_Id };

            auto failedMovePacked = PackMessage(failedMove);
			return;
		}

		GameMove gameMove{ m_Id, move };
        auto gameMovePacked = PackMessage(gameMove);
		/*
			Find a way to signal the client that they made successful move
		*/

        auto p1 = m_Player1.lock();
        auto p2 = m_Player2.lock();

        if (!p1 || !p2)
        {
            // log out error
            // possibly close this game since we have invalid players
            return;
        }

		if (p1->Id() == m_NextTurnPlayerId)
        {
            p2->Write(std::move(gameMovePacked));
            m_NextTurnPlayerId = p2->Id();
        }
		else if (p2->Id() == m_NextTurnPlayerId)
		{
			p1->Write(std::move(gameMovePacked));
            m_NextTurnPlayerId = p1->Id();
		}
        else
        {
            SC_CORE_ERROR("Game ({}) invalid NextTurnPlayerId, cannot find next turn player", m_Id);
        }

	}
}