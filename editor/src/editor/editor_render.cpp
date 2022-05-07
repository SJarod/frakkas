#include <imgui.h>
#include <ImGuizmo.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>

#include <SDL.h>
#include <glad/glad.h>

#include "engine.hpp"

#include "editor/editor_render.hpp"


using namespace Editor;

bool EditorRender::isEditingDrag = false;
Vector2 EditorRender::mouseLockPosition {};

void EditorRender::InitImGui(Engine& io_engine)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

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

        Renderer::LowLevel::Camera& cam = *io_engine.GetEditorGamera();
        float frameSpeed = 20.f * Game::Time::GetDeltaTime();

        float forwardVelocity = Game::Inputs::GetAxis("forward") * frameSpeed;
        float strafeVelocity = Game::Inputs::GetAxis("horizontal") * frameSpeed;

        Vector3 pos = cam.transform.position;
        Vector3 rot = cam.transform.rotation;

        pos.x += Maths::Sin(rot.y) * forwardVelocity + Maths::Cos(rot.y) * strafeVelocity;
        pos.z += Maths::Sin(rot.y) * strafeVelocity - Maths::Cos(rot.y) * forwardVelocity;

        rot.y += Game::Inputs::GetMouseDelta().x * 0.005f;
        rot.x -= Game::Inputs::GetMouseDelta().y * 0.005f;

        rot.y = Maths::Modulo(rot.y, Maths::Constants::doublePi);
        rot.x = Maths::Clamp(rot.x, -Maths::Constants::halfPi, Maths::Constants::halfPi);

        pos.y += frameSpeed * Game::Inputs::GetAxis("vertical");

        cam.transform.position = pos;
        cam.transform.rotation = rot;
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

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (isEditingDrag && Game::Inputs::IsReleased(Game::EButton::MOUSE_LEFT))
    {
        Engine::SetCursorGameMode(false);
        Engine::SetCursorPosition(mouseLockPosition);
        isEditingDrag = false;
    }

    // TODO Remove if no longer needed
    bool ShowDemoWindow = true;
    ImGui::ShowDemoWindow(&ShowDemoWindow);

    bool reloadScene = false;
    m_menuBar.OnImGuiRender(io_engine, reloadScene, m_hierarchy.selected);
    if (reloadScene)
    {
        m_hierarchy.selected = nullptr;
        io_engine.gameRestart = true;
        io_engine.SetRunMode(Engine::RunFlag_Editing);
    }

    m_debugger.OnImGuiRender();
    m_hierarchy.OnImGuiRender(io_engine.entityManager);
    m_console.OnImGuiRender();
    m_inspector.OnImGuiRender(m_hierarchy.selected, guizmoOperation);
    m_fileBrowser.OnImGuiRender();
    m_game.OnImGuiRender(io_engine);
    m_scene.OnImGuiRender(*io_engine.editorFBO, *io_engine.GetEditorGamera(), m_hierarchy.selected, guizmoOperation);

    UpdateImGui();
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
