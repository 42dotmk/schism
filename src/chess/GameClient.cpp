#include "GameClient.h"

#include "Common.h"
#include "Server/Messages.h"
#include "ChessEvents.h"

namespace Chess
{
    GameClient::GameClient(const std::string& host, const std::string& port, Schism::GameEvent::CallbackBus& gameEventBus)
        :
        m_GameEventBus{ gameEventBus },
        m_Socket(m_IoContext)
    {
        m_ReadBuffer.resize(Net::MAX_MESSAGE_BUFFER_LENGTH);

        asio::ip::tcp::resolver resolver(m_IoContext);

        auto endpoints = resolver.resolve(host, port);

        asio::async_connect(m_Socket, endpoints,
            [this](const asio::error_code& error, const asio::ip::tcp::endpoint& endpoint)
            {
                if (error)
                {
                    SC_CORE_ERROR("Error connecting to server!");
                }
                SC_CORE_INFO("Connected to server");
                ReadWork();
            });

        ListenGameEvent<Net::GameMove>(
                [this](Net::GameMove&& m)
            {
                auto message = Net::PackMessage(m);
                auto lengthSent = m_Socket.write_some(asio::buffer(message));
                if (lengthSent != message.size())
                {
                    SC_CORE_ERROR("(GameClient) Error sending data");
                }
            });

        ListenGameEvent<Net::RequestGame>(
                [this](Net::RequestGame&& requestGame)
            {
                std::vector<uint8_t> vec;
                Net::PrependMessageType(vec, Net::MessageType::REQUEST_GAME);

                auto lengthSent = m_Socket.write_some(asio::buffer(vec));

                if (lengthSent != vec.size())
                {
                    SC_CORE_ERROR("(GameClient) Error sending data");
                }
            });
    }

    void GameClient::PollEvents()
    {
        ProcessGameEvents();
    }

    void GameClient::Stop()
    {
        m_IoContext.stop();
    }

    void GameClient::Start()
    {
        m_IoContext.run();
    }

    void GameClient::ReadWork()
    {
        m_Socket.async_read_some(
                asio::buffer(m_ReadBuffer, Net::MAX_MESSAGE_BUFFER_LENGTH),
                [this](std::error_code ec, size_t length)
                {
                    if (ec)
                    {
                        SC_CORE_ERROR("Error reading from socket!");
                    }
                    if (length < 1)
                    {
                        SC_CORE_ERROR("Message not big enough");
                    }

                    HandleRead(length);
                    ReadWork();
                });
    }

    void GameClient::HandleRead(size_t length)
    {
        if (length == 0)
        {
            // log out error
            return;
        }
        switch (m_ReadBuffer[0])
        {
            case Net::MessageType::ENTER_GAME:
            {
                auto enterGame = Net::UnpackMessage<Net::EnterGame>(m_ReadBuffer, length);
                m_GameEventBus.PostEvent(enterGame);
                break;
            }
            case Net::MessageType::GAME_MOVE:
            {
                auto gameMove = Net::UnpackMessage<Net::GameMove>(m_ReadBuffer, length);
                m_GameEventBus.PostEvent(gameMove);
                break;
            }
            case Net::MessageType::SUCCESSFUL_MOVE:
            {
                auto successfulMove = Net::UnpackMessage<Net::SuccessfulMove>(m_ReadBuffer, length);
                m_GameEventBus.PostEvent(successfulMove);
                break;
            }
        }
    }
}