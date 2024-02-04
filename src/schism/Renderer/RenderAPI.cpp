#include "RenderAPI.h"

#include "glad/glad.h"

#include "Renderer2D.h"
#include "SpriteRenderer.h"
#include "schism/System/Log.h"

namespace Schism::Renderer
{
	void API::Init()
	{  
		glEnable(GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		// Renderer2D::Init();
		// This will be deprecated, stays here to test other parts while the batch renderer gets finished
		SpriteRenderer::Init();  
	}

	void API::Shutdown()
	{
		// Renderer2D::Shutdown();
		SpriteRenderer::Shutdown();
	}
	
	void API::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void API::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void API::SetViewport(uint32_t width, uint32_t height)
	{
		glViewport(0, 0, width, height);
	}
}
