#include <imgui.h>
#include <ImGuizmo.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>

#include <SDL.h>
#include <glad/glad.h>

#include "engine.hpp"

#include "renderer/graph.hpp"
#include "editor/editor_render.hpp"


using namespace Editor;

bool EditorRender::editingDrag = false;
bool EditorRender::editingText = false;
Vector2 EditorRender::mouseLockPosition {};
Vector2 EditorRender::gameWindowSize {};

inline void CheckEngineQuitEvent()
{
    if (Game::Inputs::quit)
    {
        ImGui::OpenPopup("QUIT");
        Game::Inputs::quit = false;
    }

    /// CREATE NEW SCENE POPUP
    if (ImGui::BeginPopupModal("QUIT", nullptr))
    {
        ImGui::Text("Are you sure to close the engine?\nDon't forget to save your current scene!\n");

        ImGui::Separator();

        if (ImGui::Button("Quit", ImVec2(120, 0)))
        {
            Game::Inputs::quit = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void EditorRender::InitImGui(Engine& io_engine)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport;

    io.Fonts->AddFontFromFileTTF("editor/assets/Louis_George_Cafe_Bold.ttf", 15.f);

    m_menuBar.FrakkasColors();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 6.0f;
        style.ChildRounding = 6.0f;
        style.FrameRounding = 3.0f;
        style.GrabRounding = 12.0f;
        style.TabRounding = 6.0f;
    }

    ImGui_ImplSDL2_InitForOpenGL(SDL_GL_GetCurrentWindow(), SDL_GL_GetCurrentContext());
    ImGui_ImplOpenGL3_Init("#version 450");

    // BIND FUNCTION TO ENGINE
    UpdateEvent editorUpdateEvent = [&](){ UpdateAndRender(io_engine);};
    io_engine.updateEventsHandler.emplace_back(editorUpdateEvent);
    UpdateEvent editorCameraUpdateEvent = [&]()
    {
        if (!m_scene.isMoving) return;

        Game::Transform& camTrs = io_engine.GetEditorCameraTransform();
        float frameSpeed = 20.f * Game::Time::GetRawDeltaTime();

        float forwardVelocity = Game::Inputs::GetAxis("forward");
        float strafeVelocity = Game::Inputs::GetAxis("horizontal");
        float verticalVelocity = Game::Inputs::GetAxis("verticalDebug");

        Vector3 rot = camTrs.rotation;

        float invertY = Maths::Abs(rot.x) > Maths::Constants::halfPi ? -1.f : 1.f;
        static float hold = 0.5f;

        Vector3 dir = Vector3::zero;

        dir.x += Maths::Sin(rot.y) * invertY * forwardVelocity + Maths::Cos(rot.y) * strafeVelocity;
        dir.y += Maths::Sin(rot.x) * forwardVelocity;
        dir.z += Maths::Sin(rot.y) * strafeVelocity - Maths::Cos(rot.y) * invertY * forwardVelocity;

        dir.x -= Maths::Sin(rot.x) * verticalVelocity * Maths::Sin(rot.y);
        dir.y += Maths::Cos(rot.x) * verticalVelocity;
        dir.z += Maths::Sin(rot.x) * verticalVelocity * Maths::Cos(rot.y);

        dir = dir.Normalize();
        if (dir.Length() > 0.f)
            hold += Game::Time::GetRawDeltaTime();
        else
            hold = 0.5f;

        rot.y += Game::Inputs::GetMouseDelta().x * 0.005f;
        rot.x -= Game::Inputs::GetMouseDelta().y * 0.005f;

        rot.x = Maths::Abs(rot.x) > Maths::Constants::pi + Maths::Constants::weakEpsilon ? -rot.x : rot.x;
        rot.y = Maths::Modulo(rot.y, Maths::Constants::doublePi);

        camTrs.position = camTrs.position.get() + dir * frameSpeed * hold * hold;
        camTrs.rotation = rot;
    };
    io_engine.updateEventsHandler.emplace_back(editorCameraUpdateEvent);

    io_engine.editorInputsEvent = std::bind(&ImGui_ImplSDL2_ProcessEvent, std::placeholders::_1);
}

void EditorRender::QuitImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void EditorRender::UpdateAndRender(Engine& io_engine)
{
    //Renderer::LowLevel::Framebuffer& io_fbo = *pio_fbo;
    //Game::EntityManager& i_entityManager = *pi_entityManager;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    bool stylePushed = false;
    if (io_engine.GetRunMode() & Utils::UpdateFlag_Gaming)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.75f);
        stylePushed = true;
    }

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    // Reset drag edit state
    if (editingDrag && Game::Inputs::IsReleased(Game::EButton::MOUSE_LEFT))
    {
        Engine::SetCursorGameMode(false);
        Engine::SetCursorPosition(mouseLockPosition);
        editingDrag = false;
    }

    // Unfocus whatever item with ESCAPE
    if (Game::Inputs::IsPressed(Game::EButton::ESCAPE))
        ImGui::SetWindowFocus(nullptr);

    // Reset editing text if needed
    if(!ImGui::IsAnyItemActive())
        Editor::EditorRender::editingText = false;

    // Edit entities with inputs
    m_hierarchy.OnImGuiRender(io_engine.entityManager);
    Inspector::OnImGuiRender(m_hierarchy.selected, guizmoOperation);

    // Disable inputs if typing
    if (editingText || !(io_engine.GetRunMode() & Utils::UpdateFlag_Editing) || m_scene.isMoving)
        io_engine.DisableInputs();

    // reload scene if menu bar call scene reloading
    bool reloadScene = false;
    MenuBar::OnImGuiRender(io_engine, reloadScene, m_hierarchy.selected);
    if (reloadScene)
    {
        m_hierarchy.selected = nullptr;
        io_engine.gameRestart = true;
        io_engine.SetRunMode(Utils::UpdateFlag_Editing);
    }

    // Render viewers
    m_debugger.OnImGuiRender();
    m_resources.OnImGuiRender();
    m_console.OnImGuiRender();
    m_fileBrowser.OnImGuiRender();

    // Render scenes
    if (m_scene.isMoving)
        io_engine.EnableInputs();

    m_scene.OnImGuiRender(io_engine, m_hierarchy.selected, guizmoOperation);

    if(stylePushed)
        ImGui::PopStyleVar();

    io_engine.EnableInputs();
    m_game.OnImGuiRender(io_engine);

    CheckEngineQuitEvent();

    UpdateImGui();

    io_engine.gameFBO->size = {gameWindowSize.x, gameWindowSize.y};
}

void EditorRender::UpdateImGui()
{
    ImVec4 clearColor = ImVec4(0.4f, 0.5f, 0.6f, 1.f);

    ImGuiIO& io = ImGui::GetIO();

    ImGui::Render();

    glViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}
