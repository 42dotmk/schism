#pragma once

#include "asio.hpp"
#include "Common.h"
#include "schism/Game/GameEvent/CallbackListener.h"
#include "schism/Game/GameEvent/CallbackBus.h"

namespace Chess
{
class GameClient: public Schism::GameEvent::CallbackListener
    {
    public:
        explicit GameClient(const std::string& host, const std::string& port, Schism::GameEvent::CallbackBus& gameEventBus);

        void Stop();

        void Start();
        void PollEvents();
    private:
        void ReadWork();
        void HandleRead(size_t length);
        std::vector<uint8_t> m_ReadBuffer;
        Schism::GameEvent::CallbackBus& m_GameEventBus;
        asio::io_context m_IoContext;
        asio::ip::tcp::socket m_Socket;
    };
}