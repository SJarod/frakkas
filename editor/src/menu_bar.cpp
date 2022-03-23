//
// Created by m.mehalin on 16/03/2022.
//

#include <imgui.h>
#include <cstdlib>

#include "editor/menu_bar.hpp"

using namespace Editor;


void MenuBar::OnImGuiRender()
{
    ImGui::BeginMainMenuBar();

    FileField();

    EditField();

    OptionsField();

    ImGui::EndMainMenuBar();
}

void MenuBar::FileField()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Scene", "CTRL+N")) {}
        if (ImGui::MenuItem("Open Scene", "CTRL+O")) {}
        ImGui::Separator();

        if (ImGui::MenuItem("Save", "CTRL+S")) {}
        ImGui::Separator();

        if (ImGui::MenuItem("New Project...")) {}
        if (ImGui::MenuItem("Open Project...")) {}
        if (ImGui::MenuItem("Save Project")) {}
        ImGui::Separator();

        if (ImGui::MenuItem("Quit", "ALT+F4")) {exit(0);}

        ImGui::EndMenu();
    }
}

void MenuBar::EditField()
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

        if (ImGui::MenuItem("Play", "CTRL+P")) {}
        if (ImGui::MenuItem("Pause", "CTRL+SHIFT+P")) {}

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
