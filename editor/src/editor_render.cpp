//
// Created by f.marcellot on 16/03/2022.
//

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>

#include <SDL.h>
#include <glad/glad.h>

#include "renderer/lowlevel/lowrenderer.hpp"

#include "editor/editor_render.hpp"

using namespace Editor;

void EditorRender::InitImGui()
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

    ImGui_ImplSDL2_InitForOpenGL(SDL_GL_GetCurrentWindow(), SDL_GL_GetCurrentContext());
    ImGui_ImplOpenGL3_Init("#version 450");
}

void EditorRender::QuitImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void EditorRender::UpdateAndRender(const Renderer::LowLevel::Framebuffer& io_fbo)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    m_menuBar.OnImGuiRender();
    m_hierarchy.OnImGuiRender();
    m_console.OnImGuiRender();
    m_inspector.OnImGuiRender();
    m_resourceViewer.OnImGuiRender();
    m_game.OnImGuiRender();
    m_scene.OnImGuiRender(reinterpret_cast<ImTextureID>(io_fbo.color));

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
