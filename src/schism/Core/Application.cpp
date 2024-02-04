#include "Application.h"

#include "imgui.h"
#include "Window.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "schism/Renderer/RenderAPI.h"
#include "schism/System/Log.h"
#include "schism/System/System.h"
#include "schism/Core/Events/WindowEvents.h"
#include "schism/Renderer/Renderer2D.h"
#include "AL/al.h"
#include "AL/alc.h"

namespace Schism
{
	Application::Application(int w, int h, const char* name)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		Ref<Core::Window> Window = MakeRef<Core::Window>();

		
		Window->Create(w, h, name);

    
		Window->SetEventCallback([this](Event&& e)
			{
				OnEvent(e);
			});
		m_Ctx = CreateSharedContext(Window);

    
		m_Ctx->SceneManager.Switch = [this](const std::string& name)
			{
				m_SceneManager.Switch(name);
			};
		m_Ctx->SceneManager.Destroy = [this](const std::string& name)
			{
				m_SceneManager.Destroy(name);
			};

		m_SceneManager.InitContext(m_Ctx);
		
		Renderer::API::Init();
    
        ALCdevice* aldevice = alcOpenDevice(nullptr);
        ALCcontext* context = alcCreateContext(aldevice, nullptr);

        if (!alcMakeContextCurrent(context))
        {
            SC_CORE_ERROR("Cannot create context");
            return;
        }

		SC_CORE_INFO("Schism succesfully initialized");
		//SC_CORE_INFO("Gpu - {0} {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
		//SC_CORE_INFO("Driver - {0}", glGetString(GL_VERSION));
		//SC_CORE_INFO("Shader Version - {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
		SC_CORE_INFO("Processor count - {0}", std::thread::hardware_concurrency());

		// TEMPPPPP !!!
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(Window->GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 400");
	}

	Application::~Application()
	{
		Renderer::API::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		EventHandler evt(e);

		CLASSEVENT(evt, WindowResizeEvent)
		{
			Renderer::API::SetViewport(e.GetWidth(), e.GetHeight());
		});
		
		m_SceneManager.OnSystemEvent(e);
	}
	
	void Application::Run()
	{
		// Boilerplate code
		auto winPtr = m_Ctx->window->GetNativeWindow();
		
		auto StartTime = std::chrono::high_resolution_clock::now();
		auto LastFrameTime = StartTime;
            
		Timestep ts;
		while (!glfwWindowShouldClose(winPtr))
		{
			StartTime = std::chrono::high_resolution_clock::now();
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(StartTime - LastFrameTime).count();
			LastFrameTime = StartTime;
			ts = ms * 1.f / 1000;

			Renderer::API::Clear();
			m_Ctx->window->ProcessEvents();
			m_SceneManager.OnUpdate(ts);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

			m_SceneManager.OnDraw();
            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			m_Ctx->window->Swap();
		}

		SC_CORE_INFO("Schism shutting down");
		
		exit(0);
	}

}
