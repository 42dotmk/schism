#pragma once

#include <parallel_hashmap/phmap.h>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <memory>
#include <thread>
#include <vector>

#include "Client.h"
#include "Game.h"
#include "Messages.h"

namespace Chess::Net {
class Server {
    public:
        explicit Server(short port);
        ~Server();

        void Start();
        void Stop();

    private:
        void Accept();

        void HandleClientDisconnect();  // todo
        void HandleClientRead(const std::weak_ptr<Client>& client,
                              std::vector<uint8_t>& readBuffer,
                              std::size_t length);
        void CreateGame(const std::shared_ptr<Client>& clientThatStarted);

        asio::io_context m_Context;
        asio::ip::tcp::acceptor m_Acceptor;
        phmap::parallel_flat_hash_map<std::uint32_t, std::shared_ptr<Client>>
            m_Clients;
        phmap::parallel_flat_hash_map<std::uint32_t, std::shared_ptr<Game>>
            m_ActiveGames;
};
}  // namespace Chess::Net