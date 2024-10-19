#include "Application.h"

#include "AL/al.h"
#include "AL/alc.h"
#include "GLFW/glfw3.h"
#include "Window.h"
#include "bgfx/defines.h"
#include "bgfx/platform.h"
#include "imgui.h"
#include "imconfig.h"
#include "imgui_impl_glfw.h"
#include "schism/Core/EventHandlers/EventManager.h"
#include "schism/Core/EventHandlers/KeyboardEventHandler.h"
#include "schism/Core/EventHandlers/MouseEventHandler.h"
#include "schism/Core/Events/Event.h"
#include "schism/Core/Events/WindowEvents.h"
#include "schism/Renderer/RenderAPI.h"
#include "schism/Renderer/Renderer2D.h"
#include "schism/System/Log.h"
#include "schism/System/System.h"

#include <bgfx/bgfx.h>
#include <imgui_impl_bgfx.h>
#include <glm/gtc/constants.hpp>
#include <string>

namespace Schism {
Application::Application(int w, int h, const char* name) {
    Core::WindowRef Window = MakeRef<Core::Window>();

    SetupEventHandlers();

    // we have to attach the event adapter before we create the window
    // in the future possibly pass this into Create(w, h, name)
    // Multi window can support multiple event propagation schemes
    Window->Create(w, h, name, m_EventManager);

    m_Ctx = CreateSharedContext(Window);

    m_Ctx->SceneManager.Switch = [this](const std::string& name) {
        m_SceneManager.Switch(name);
    };
    m_Ctx->SceneManager.Destroy = [this](const std::string& name) {
        m_SceneManager.Destroy(name);
    };

    m_SceneManager.InitContext(m_Ctx);

    SC_CORE_ERROR("Got here before window");
    auto renderType = Renderer::API::Init(Window);

    SC_CORE_ERROR("Got here after window");
    ALCdevice* aldevice = alcOpenDevice(nullptr);
    ALCcontext* context = alcCreateContext(aldevice, nullptr);

    if (!alcMakeContextCurrent(context)) {
        SC_CORE_ERROR("Cannot create context");
        return;
    }

    /*auto vendor =*/
    /*    std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));*/
    /*auto renderer =*/
    /*    std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));*/
    /*auto version =*/
    /*    std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));*/
    /*auto shader_version = std::string(reinterpret_cast<const char*>(*/
    /*    glGetString(GL_SHADING_LANGUAGE_VERSION)));*/
    /**/
    /*SC_CORE_INFO("Schism succesfully initialized");*/
    /*SC_CORE_INFO("Gpu - {0} {1}", vendor, renderer);*/
    /*SC_CORE_INFO("Driver - {0}", version);*/
    /*SC_CORE_INFO("Shader Version - {0}", shader_version);*/
    /*SC_CORE_INFO("Processor count - {0}", std::thread::hardware_concurrency());*/
    /**/
    // TEMPPPPP !!!
    //

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize.x = Window->GetWidth();
    io.DisplaySize.y = Window->GetHeight();

    (void)io;

    ImGui::StyleColorsDark();
    ImGui_Implbgfx_Init(0);

    auto* glfwWindow = Window->GetGLFWWindow();
    switch (*renderType) {
        case bgfx::RendererType::Vulkan:
            ImGui_ImplGlfw_InitForVulkan(glfwWindow, true);
            break;

        case bgfx::RendererType::OpenGL:
            ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
            break;

        default:
            ImGui_ImplGlfw_InitForOther(glfwWindow, true);
    }
}

Application::~Application() {
    Renderer::API::Shutdown();
}

void Application::OnEvent(Event& e) {
    EventHandler evt(e);

    CLASSEVENT(evt, WindowResizeEvent) {
        SC_CORE_WARN("Window resized");
        /*m_Ctx->window->Resize(e.GetWidth(), e.GetHeight());*/
        Renderer::API::SetViewport(e.GetWidth(), e.GetHeight());
    });

    m_SceneManager.OnSystemEvent(e);
}

void Application::Run() {
    // Boilerplate code
    auto winPtr = m_Ctx->window->GetGLFWWindow();

    auto StartTime = std::chrono::high_resolution_clock::now();
    auto LastFrameTime = StartTime;

    Timestep ts;
    while (!glfwWindowShouldClose(winPtr)) {
        StartTime = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      StartTime - LastFrameTime)
                      .count();
        LastFrameTime = StartTime;
        ts = ms * 1.f / 1000;

        Renderer::API::Clear();
        m_Ctx->window->ProcessEvents();
        /*m_SceneManager.OnUpdate(ts);*/

        bgfx::touch(0);

        /*ImGui::NewFrame();*/
        bgfx::dbgTextPrintf(
            10, 10, 0x0f,
            std::format("Testing bgfx, {}", ts.GetMiliseconds()).c_str());
        /*m_SceneManager.OnDraw();*/

        /*ImGui::Render();*/
        /*bgfx::setDebug(BGFX_DEBUG_TEXT);*/
        bgfx::frame();

        ImGui_Implbgfx_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());

        m_Ctx->window->Swap();
    }

    SC_CORE_INFO("Schism shutting down");

    exit(0);
}

void Application::SetupEventHandlers() {
    auto eventManager = MakeRef<Core::EventManager>();

    auto keyboardEventHandler = MakeRef<Core::KeyboardEventHandler>();
    auto mouseEventHandler = MakeRef<Core::MouseEventHandler>();

    eventManager->AttachAdapter(EventCategory::Mouse, mouseEventHandler);
    eventManager->AttachAdapter(EventCategory::Keyboard, keyboardEventHandler);

    m_EventManager = eventManager;

    m_EventManager->Listener.connect([this](Event&& e) { OnEvent(e); });
}
}  // namespace Schism
