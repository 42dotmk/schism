#pragma once

#include "Client.h"
#include "chess/Engine.h"

namespace Chess::Net
{
    class Game
    {
    private:
        explicit Game(std::weak_ptr<Client> player1, std::weak_ptr<Client> player2, bool player1Turn);
    public:


        static std::shared_ptr<Game> Create(std::weak_ptr<Client> p1, std::weak_ptr<Client> p2, bool player1Turn);

        void DisconnectPlayer(uint64_t player)
        {
            // todo
        }
        void MakeMove(const Move& m, const std::weak_ptr<Client>& clientThatSentMove);
        uint64_t Id()
        {
            return m_Id;
        }

    private:
        Engine m_GameState;
        uint64_t m_Id;
        uint64_t m_NextTurnPlayerId;
        std::weak_ptr<Client> m_Player1;
        std::weak_ptr<Client> m_Player2;
    };
}
