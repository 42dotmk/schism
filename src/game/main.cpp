#include <thread>
#include "Game.h"
#include "asio.hpp"
#include "chess/ChessEvents.h"
#include "chess/Common.h"
#include "chess/Server/Server.h"
#include "msgpack/msgpack.hpp"
#include "schism/Game/GameEvent/CallbackBus.h"
#include "schism/Game/GameEvent/CallbackListener.h"
#include "schism/Game/GameEvent/SyncListener.h"
#include "schism/Sandbox/Sandbox.h"
#include "schism/Schism.h"

int main(int argc, char* argv[]) {
    Schism::Init();

    Schism::Sandbox sandbox;

    sandbox.Run();

    return 0;
}
