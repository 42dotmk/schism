#include "Game.h"

#include "chess/Chess.h"
#include "schism/Audio/Source.h"
#include "schism/Audio/WavLoader.h"
#include "schism/System/Log.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <chrono>
#include <filesystem>
#include <thread>
#include "schism/Sandbox/SampleScene.h"

void checkForError()
{
    auto err = alGetError();

    if (err != AL_NO_ERROR)
    {
        SC_CORE_ERROR("Open AL ERROR {}", err);
    }
}
Game::Game()
	:
	Application(800, 800, "Chess")
{
	m_Ctx->GlobalAssets.Shaders.Load("spriterenderer", "res/shaders/sprite_renderer.vert", "res/shaders/sprite_renderer.frag");
    m_Ctx->GlobalAssets.Audio.Load("testaudio", "res/sample-3s.wav");

	SpriteRenderer::RegisterShader(m_Ctx->GlobalAssets.Shaders.GetHandle("spriterenderer"));
    m_SceneManager.Register<SampleScene>("sample scene");
	// m_SceneManager.Register<Chess::Chess>("chess");

    ALfloat listenerOri[] = { 0.f, 0.f, 1.f, 0.f, 1.f, 0.f };

    alListener3f(AL_POSITION, 0, 0, 1.f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listenerOri);
    SC_CORE_INFO("Playing audio!");
    m_Ctx->GlobalAssets.Audio.GetHandle("testaudio")->Play();
}

Game::~Game()
{
	
}
