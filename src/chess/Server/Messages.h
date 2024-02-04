#pragma once

#include <cstdint>
#include "chess/Common.h"
#include "msgpack/msgpack.hpp"
#include <type_traits>

namespace Chess::Net
{

    enum MessageType: std::uint8_t
    {
        LOGIN = 0,
        LOGOUT,
        ENTER_GAME,
        REQUEST_GAME,
        GAME_MOVE,
        SUCCESSFUL_MOVE,
        FAILED_MOVE,
        COUNT
    };

    inline void PrependMessageType(std::vector<uint8_t>& vec, MessageType type) noexcept
    {
        vec.insert(vec.begin(), type);
    }

    template<typename T>
    inline T UnpackMessage(const std::vector<uint8_t>& buffer, size_t length)
    {
        return msgpack::unpack<T>(&buffer[1], length - 1);
    }

    template<typename T>
    inline std::vector<uint8_t> PackMessage(T& data)
    {
        auto packed = msgpack::pack(data);
        PrependMessageType(packed, data.Type());
        return std::move(packed);
    }

    struct RequestGame
    {
        int k{ 0 };
    };

    struct EnterGame
    {
        uint64_t gameId;
        uint64_t otherPlayerId; // might be needed
        bool isWhite;

        inline static constexpr MessageType Type()
        {
            return ENTER_GAME;
        }

        template<typename T>
        void pack(T& p)
        {
            p(gameId, otherPlayerId, isWhite);
        }
    };

    struct GameMove
    {
        uint64_t gameId{ 0 };
        Move move;

        inline static constexpr MessageType Type()
        {
            return GAME_MOVE;
        }

        template<typename T>
        void pack(T& p)
        {
            p(gameId, move);
        }
    };

    struct SuccessfulMove
    {
        uint16_t moveCount{ 0 };
        uint64_t gameId{ 0 };

        inline static constexpr MessageType Type()
        {
            return SUCCESSFUL_MOVE;
        }

        template<typename T>
        void pack(T& p)
        {
            p(moveCount, gameId);
        }
    };

    struct FailedMove
    {
        uint64_t gameId{ 0 };

        inline static constexpr MessageType Type()
        {
            return FAILED_MOVE;
        }

        template<typename T>
        void pack(T& p)
        {
            p(gameId);
        }
    };
}
