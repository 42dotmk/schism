#include "Game.h"

int main(int argc, char* argv[])
{
    Schism::Init();

    Schism::Sandbox sandbox;
    sandbox.Run();

    return 0;
}
