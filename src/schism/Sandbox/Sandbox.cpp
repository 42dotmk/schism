#include "Sandbox.h"

#include "SampleScene.h"

namespace Schism
{
	Sandbox::Sandbox()
		:
		Application(1280, 720, "Sandbox")
	{
		m_Ctx->GlobalAssets.Textures.Load("ship1sprite", "res/ships/1.png");
		m_Ctx->GlobalAssets.Shaders.Load("spriterenderer", "res/shaders/sprite_renderer.vert", "res/shaders/sprite_renderer.frag");
		
		m_SceneManager.Register<SampleScene>("samplescene");
	}

	Sandbox::~Sandbox()
	{
		
	}
}
