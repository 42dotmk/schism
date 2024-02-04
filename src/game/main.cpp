#include "schism/Sandbox/Sandbox.h"
#include "schism/Schism.h"
#include "Game.h"
#include "schism/Game/GameEvent/SyncListener.h"
#include "schism/Game/GameEvent/CallbackListener.h"
#include "schism/Game/GameEvent/CallbackBus.h"
#include "msgpack/msgpack.hpp"
#include "chess/Common.h"
#include "chess/ChessEvents.h"
#include "chess/Server/Server.h"
#include "asio.hpp"
#include <thread>

int main(int argc, char* argv[])
{
    Schism::Init();

    Schism::Sandbox sandbox;

    sandbox.Run();

    return 0;
}
