#include <imgui.h>
#include <cstdlib>

#include "game/inputs_manager.hpp"

#include "renderer/graph.hpp"

#include "helpers/game_edit.hpp"
#include "editor/menu_bar.hpp"


using namespace Editor;
using namespace Game;


void MenuBar::OnImGuiRender(Renderer::Graph& io_graph, bool& o_gaming, bool& o_loadScene)
{
    ImGui::BeginMainMenuBar();

    FileField(io_graph, o_loadScene);

    EditField(o_gaming);

    if (Inputs::IsPressed(EButton::P))
        o_gaming = !o_gaming;

    OptionsField();

    ImGui::EndMainMenuBar();
}

void MenuBar::FileField(Renderer::Graph& io_graph, bool& o_loadScene)
{
    std::string fileAction = "None";

    auto SetNewSceneAction = [&fileAction]() { fileAction = "New scene"; };
    auto SetOpenSceneAction = [&fileAction]() { fileAction = "Open scene"; };
    auto SaveScene = [&io_graph]() { io_graph.SaveScene(); };

#pragma region Menu item
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Scene", "CTRL+N"))
            SetNewSceneAction();

        if (ImGui::MenuItem("Open Scene", "CTRL+O"))
            SetOpenSceneAction();

        ImGui::Separator();

        if (ImGui::MenuItem("Save", "CTRL+S"))
            SaveScene();

        if (ImGui::MenuItem("Reload Scene"))
        {
            o_loadScene = true;
            io_graph.ReloadScene();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("New Project...")) {}
        if (ImGui::MenuItem("Open Project...")) {}
        if (ImGui::MenuItem("Save Project")) {}
        ImGui::Separator();

        if (ImGui::MenuItem("Quit"))
            Inputs::quit = true;

        ImGui::EndMenu();
    }
#pragma endregion

#pragma region Inputs shortcut
    if(Inputs::IsControlCommandPressed(EButton::N)) 
        SetNewSceneAction();
    if (Inputs::IsControlCommandPressed(EButton::O))
        SetOpenSceneAction();
    if (Inputs::IsControlCommandPressed(EButton::S))
        SaveScene();
#pragma endregion

#pragma region Popup
    if (fileAction == "New scene")
        ImGui::OpenPopup("Create new scene?");
    if (fileAction == "Open scene")
        ImGui::OpenPopup("Open scene?");

    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    /// CREATE NEW SCENE POPUP
    if (ImGui::BeginPopupModal("Create new scene?", NULL))
    {
        ImGui::Text("Are you sure to open a new scene?\nDon't forget to save your current scene!\n");

        ImGui::Separator();

        //            static bool reminder = false;
        //            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        //            ImGui::Checkbox("Don't ask me next time", &reminder);
        //            ImGui::PopStyleVar();

        static std::string sceneName = "new_scene";
        Helpers::Edit(sceneName, "Name");

        if (ImGui::Button("Create", ImVec2(120, 0)))
        {
            std::ofstream emptyFile(io_graph.GetSceneFullPath(sceneName));
            emptyFile.close();
            io_graph.LoadScene(sceneName);
            o_loadScene = true;
            sceneName = "new_scene";
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();

        ImGui::EndPopup();
    }

    /// OPEN EXISTING SCENE POPUP
    if (ImGui::BeginPopupModal("Open scene?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure to open a new scene?\nDon't forget to save your current scene!\n");

        ImGui::Separator();

        static std::string sceneName = "exemple_scene";
        static bool exist = true;
        Helpers::Edit(sceneName, "Name");

        if (ImGui::Button("Open", ImVec2(120, 0)))
        {
            std::ifstream file(io_graph.GetSceneFullPath(sceneName));
            if (file.is_open())
            {
                io_graph.LoadScene(sceneName);

                sceneName = "exemple_scene";
                exist = true;
                ImGui::CloseCurrentPopup();
            }
            else
                exist = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            sceneName = "exemple_scene";
            exist = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();

        if (!exist)
            ImGui::Text("Input name does not have existing scene file...");

        ImGui::EndPopup();
    }
#pragma endregion
}

void MenuBar::EditField(bool& o_gaming)
{
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        ImGui::Separator();

        if (ImGui::MenuItem("Select All", "CTRL+A")) {}
        if (ImGui::MenuItem("Deselect All", "CTRL+D")) {}
        ImGui::Separator();

        if (ImGui::MenuItem("Play", "P")) { o_gaming = true; }
        if (ImGui::MenuItem("Pause", "P")) { o_gaming = false; }

        ImGui::EndMenu();
    }
}

void MenuBar::OptionsField()
{
    if (ImGui::BeginMenu("Options"))
    {
        if (ImGui::BeginMenu("Theme"))
        {
            if (ImGui::MenuItem("Dark theme"))
                ImGui::StyleColorsDark();

            if (ImGui::MenuItem("Light theme"))
                ImGui::StyleColorsLight();

            if (ImGui::MenuItem("Classic theme"))
                ImGui::StyleColorsClassic();

            if (ImGui::MenuItem("Frakkas theme"))
                MenuBar::FrakkasColors();

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

void MenuBar::FrakkasColors()
{
    ImVec4* colors = ImGui::GetStyle().Colors;

    ImGui::StyleColorsDark();

    colors[ImGuiCol_WindowBg] = ImVec4(40.f / 255.f, 40.f / 255.f, 40.f / 255.f, 1.f);
    colors[ImGuiCol_ChildBg] = ImVec4(40.f / 255.f, 40.f / 255.f, 40.f / 255.f, 1.f);
    colors[ImGuiCol_PopupBg] = ImVec4(40.f / 255.f, 40.f / 255.f, 40.f / 255.f, 1.f);

    colors[ImGuiCol_FrameBg]        = ImVec4(122.f / 255.f, 41.f / 255.f, 41.f / 255.f, 138.f / 255.f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 102.f / 255.f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 171.f / 255.f);

    colors[ImGuiCol_TitleBgActive]  = ImVec4(122.f / 255.f, 41.f / 255.f, 41.f / 255.f, 1.f);

    colors[ImGuiCol_CheckMark] = ImVec4(1.f, 1.f, 1.f, 1.f);

    colors[ImGuiCol_SliderGrab]         = ImVec4(128.f / 255.f, 128.f / 255.f, 128.f / 255.f, 1.f);
    colors[ImGuiCol_SliderGrabActive]   = ImVec4(1.f, 1.f, 1.f, 1.f);

    colors[ImGuiCol_Button]         = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 151.f / 255.f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 1.f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(250.f / 255.f, 15.f / 255.f, 15.f / 255.f, 1.f);

    colors[ImGuiCol_Header]         = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 79.f / 255.f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 204.f / 255.f);
    colors[ImGuiCol_HeaderActive]   = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 1.f);

    colors[ImGuiCol_ResizeGrip]         = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 51.f / 255.f);
    colors[ImGuiCol_ResizeGripHovered]  = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 171.f / 255.f);
    colors[ImGuiCol_ResizeGripActive]   = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 242.f / 255.f);

    colors[ImGuiCol_Tab]                = ImVec4(148.f / 255.f, 46.f / 255.f, 46.f / 255.f, 220.f / 255.f);
    colors[ImGuiCol_TabHovered]         = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 204.f / 255.f);
    colors[ImGuiCol_TabActive]          = ImVec4(173.f / 255.f, 51.f / 255.f, 51.f / 255.f, 1.f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(108.f / 255.f, 35.f / 255.f, 35.f / 255.f, 1.f);

    colors[ImGuiCol_DockingPreview] = ImVec4(250.f / 255.f, 66.f / 255.f, 66.f / 255.f, 179.f / 255.f);
}