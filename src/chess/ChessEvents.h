#pragma once
#include "msgpack/msgpack.hpp"

namespace Chess
{
    struct ResetGame{};

    struct StartGame
    {
        bool IsWhite{ true };
    };
}