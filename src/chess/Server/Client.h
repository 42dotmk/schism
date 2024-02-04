#pragma once

#include <vector>
#include <asio/ts/internet.hpp>
#include <asio/buffer.hpp>
#include "schism/Game/GameEvent/CallbackListener.h"
namespace Chess::Net
{

    /*
        Client should store a list of all the game is is active in
    */
    
    class Client
    {
        using ReadCallback = std::function<void(std::vector<uint8_t>&, std::size_t lenght)>;
    public:
        explicit Client(asio::ip::tcp::socket soc);
        ~Client();

        void Stop();

        void AssignId(uint64_t id);

        void AttachReadCallback(ReadCallback&& readCallback);

        [[nodiscard]] uint64_t Id() const
        {
            return m_Id;
        }

        void Write(std::vector<unsigned char>&& message);
    private:
        void HandleRead(size_t length);
        void ReadWork();

        uint64_t m_Id;
        asio::ip::tcp::socket m_Soc;
        std::vector<uint8_t> m_ReadBuffer;
        ReadCallback m_ReadCallback;
    };
}