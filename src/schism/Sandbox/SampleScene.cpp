#include "SampleScene.h"

#include <glm/ext/matrix_transform.hpp>
#include "glm/matrix.hpp"
#include "glm/glm.hpp"
#include "schism/Components/Sprite.h"
#include "schism/Components/Transform2D.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ImGuizmo.h"
#include "schism/Renderer/OrthographicCamera.h"
#include "schism/Scripting/Lua/Bindings/Game/Ecs/Bundle.h"
#include "schism/Scripting/Lua/Bindings/Game/Ecs/Plugin.h"
#include "schism/Scripting/Lua/Lua.h"
#include "schism/System/Log.h"
#include <glm/gtc/type_ptr.hpp>

using namespace Schism;

SampleScene::SampleScene(Core::SharedContextRef ctx, const std::string& name)
	:
	SceneBase(ctx,name),
	m_Camera(0, m_Ctx->window->GetWidth(), m_Ctx->window->GetHeight(), 0)
{
	m_Ship1 = m_Registry.create();


    Scripting::Lua::LuaState state;

    state.RegisterBinding<Scripting::Lua::Bundle>(m_Registry);
    state.RegisterBinding<Components::Transform2D>();
    state.RegisterBinding<Scripting::Lua::Plugin>();

    SC_CORE_WARN("STARTING");
    state.LoadScriptFile("res/scripts/test.lua");
}

SampleScene::~SampleScene() = default;

void SampleScene::OnAttach()
{

	m_Renderer.RegisterShader(m_Ctx->GlobalAssets.Shaders.GetHandle("spriterenderer"));
	m_Registry.emplace<Components::Sprite>(m_Ship1, m_Ctx->GlobalAssets.Textures.GetHandle("ship1sprite"));
	auto& transfrom = m_Registry.emplace<Components::Transform2D>(m_Ship1);
	transfrom.position = { 200.f, 200.f, 0.f };
	transfrom.scale = { 200.f, 200.f };
}

void SampleScene::OnDetach()
{

}

void SampleScene::OnPause()
{

}

void SampleScene::OnResume()
{

}

void SampleScene::OnSystemEvent(Event& e)
{

}

void SampleScene::OnUpdate(Timestep ts)
{

}

void EditTransform(const Renderer::OrthographicCamera& camera, const glm::mat4& matrix, Components::Transform2D& transform)
{
    #pragma region ImGuizmo
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    if (ImGui::IsKeyPressed(ImGuiKey_W))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_E))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    #pragma endregion
    
    float matrixTranslation[3], matrixRotation[3], matrixScale[3];

    if (mCurrentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;
    }

    static bool useSnap(false);
    if (ImGui::IsKeyPressed(ImGuiKey_S))
        useSnap = !useSnap;
    ImGui::Checkbox("Use Snap", &useSnap);
    ImGui::SameLine();
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

    auto view = glm::value_ptr(camera.GetViewMatrix());
    auto projection = glm::value_ptr(camera.GetProjectionMatrix());
    ImGuizmo::Manipulate(view, projection, mCurrentGizmoOperation, mCurrentGizmoMode, (float*) glm::value_ptr(matrix), NULL, NULL);

    ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(matrix), matrixTranslation , matrixRotation, matrixScale);
    ImGui::InputFloat3("Tr", matrixTranslation);
    ImGui::InputFloat3("Rt", matrixRotation);
    ImGui::InputFloat3("Sc", matrixScale);

    transform.position.x = matrixTranslation[0];
    transform.position.y = matrixTranslation[1];
    transform.position.z = matrixTranslation[2];
    
    transform.rotation = matrixRotation[2];

    transform.scale.x = matrixScale[0];
    transform.scale.y = matrixScale[1];
}

void SampleScene::OnDraw()
{
	auto view = m_Registry.view<Components::Transform2D, Components::Sprite>();
    int i = 0;
    ImGuizmo::SetOrthographic(true);
    ImGuizmo::BeginFrame();
	for (auto e : view)
	{
		const auto& [transfrom, sprite] = m_Registry.get<Components::Transform2D, Components::Sprite>(e);
        glm::mat4 mat = glm::translate(glm::mat4(1.f), transfrom.position);
		mat = glm::rotate(mat, glm::radians(transfrom.rotation), glm::vec3(0.f, 0.f, 1.f));
        mat = glm::scale(mat, glm::vec3(transfrom.scale, 1.f));

        ImGuizmo::SetID(i);

        // ImGuizmo::DrawCubes(glm::value_ptr(m_Camera.GetViewMatrix()), glm::value_ptr(m_Camera.GetProjectionMatrix()), glm::value_ptr(mat), 1);
		m_Renderer.Draw(transfrom, sprite, m_Camera.GetViewProjectionMatrix());
        EditTransform(m_Camera, mat, transfrom);
        i++;
	}
}
