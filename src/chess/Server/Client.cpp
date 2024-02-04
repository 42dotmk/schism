#include "Client.h"

#include <iostream>
#include <msgpack/msgpack.hpp>
#include <utility>
#include "chess/Common.h"

namespace Chess::Net
{
    Client::Client(asio::ip::tcp::socket soc):
        m_Soc(std::move(soc))
    {
        m_ReadBuffer.resize(MAX_MESSAGE_BUFFER_LENGTH);
        ReadWork();
    }

    Client::~Client()
    {
        Stop();
    }

    void Client::AssignId(uint64_t id)
    {
        m_Id = id;
    }

    void Client::Stop()
    {
        m_Soc.cancel();
    }

    void Client::AttachReadCallback(ReadCallback&& readCallback)
    {
        m_ReadCallback = std::move(readCallback);
    }

    void Client::Write(std::vector<unsigned char>&& message)
    {
        m_Soc.async_write_some(asio::buffer(message), [this](std::error_code, size_t length)
        {
            SC_CORE_INFO("Client {} Write", m_Id);
        });
    }

    void Client::ReadWork()
    {
        m_Soc.async_read_some(asio::buffer(m_ReadBuffer, MAX_MESSAGE_BUFFER_LENGTH),
                              [this](std::error_code ec, size_t length)
                              {
                                    if (!ec) // TODO: handle multiple errors
                                    {
                                        if (m_ReadCallback)
                                        {
                                            m_ReadCallback(m_ReadBuffer, length);
                                        }

                                        ReadWork();
                                    }
                                    else
                                    {
                                        SC_CORE_WARN("Error on read connection!");
                                    }
                              });
    }
}