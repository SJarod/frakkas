#include <imgui.h>

#include "game/component_generator.hpp"
#include "game/entity_manager.hpp"
#include "game/entity.hpp"

#include "resources/serializer.hpp"

#include "renderer/graph.hpp"

#include "helpers/game_edit.hpp"
#include "helpers/string_helpers.hpp"
#include "helpers/path_constants.hpp"

#include "editor/menu_bar.hpp"


using namespace Editor;
using namespace Game;


void MenuBar::OnImGuiRender(Engine& io_engine, bool& o_loadScene, Game::Entity* io_selectedEntity)
{
    ImGui::BeginMainMenuBar();

    FileField(*io_engine.graph, o_loadScene);

    EditField(io_engine.entityManager, io_selectedEntity);

    Engine::RunFlag rFlag = io_engine.GetRunMode();
    if (Inputs::IsPressed(EButton::P))
    {
        if (rFlag & Engine::RunFlag_Editing)
            io_engine.SetRunMode(Engine::RunFlag_Editing | Engine::RunFlag_Gaming);
        else
            io_engine.SetRunMode(Engine::RunFlag_Editing);
    }

    OptionsField();

    GameField(io_engine, o_loadScene);

    LightingField(*io_engine.graph);

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

    o_loadScene |= CreateScenePopup(io_graph);
    o_loadScene = o_loadScene || OpenScenePopup(io_graph);
#pragma endregion
}

bool MenuBar::CreateScenePopup(Renderer::Graph& io_graph)
{
    /// CREATE NEW SCENE POPUP
    if (ImGui::BeginPopupModal("Create new scene?", nullptr))
    {
        ImGui::Text("Are you sure to open a new scene?\nDon't forget to save your current scene!\n");

        ImGui::Separator();

        static std::string sceneName = "new_scene";
        Helpers::Edit(sceneName, "Name");                                       // GET SCENE NAME
        ImGui::SetItemDefaultFocus();

        if (ImGui::Button("Create", ImVec2(120, 0)))
        {
            std::ofstream emptyFile(Renderer::Graph::GetSceneFullPath(sceneName)); // CREATE BUTTON
            if (!emptyFile.is_open())
                Log::Warning("bah non");

            emptyFile.close();
            io_graph.LoadScene(sceneName);
            sceneName = "new_scene";
            ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
            return true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))                        // CANCEL BUTTON
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    return false;
}

bool MenuBar::OpenScenePopup(Renderer::Graph& io_graph)
{
    /// OPEN EXISTING SCENE POPUP
    if (ImGui::BeginPopupModal("Open scene?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure to open a new scene?\nDon't forget to save your current scene!\n");

        ImGui::Separator();

        static std::string sceneName = "exemple_scene";
        static bool exist = true;
        Helpers::Edit(sceneName, "Name");                                   // GET SCENE NAME
        ImGui::SetItemDefaultFocus();

        if (ImGui::Button("Open", ImVec2(120, 0)))                    // OPEN BUTTON
        {
            std::ifstream file(Renderer::Graph::GetSceneFullPath(sceneName));
            if (file.is_open())
            {
                io_graph.LoadScene(sceneName);

                sceneName = "exemple_scene";
                exist = true;
                ImGui::CloseCurrentPopup();

                ImGui::EndPopup();
                return true;
            }
            else
                exist = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))                    // CANCEL BUTTON
        {
            sceneName = "exemple_scene";
            exist = true;
            ImGui::CloseCurrentPopup();
        }

        if (!exist)
            ImGui::Text("Input name does not have existing scene file...");         // WARNING POPUP LOG

        ImGui::EndPopup();
    }

    return false;
}


void MenuBar::EditField(Game::EntityManager& io_entityManager, Game::Entity* io_selectedEntity)
{
    auto copyFunc = [&io_selectedEntity](){
        if (io_selectedEntity)
        {
            std::ofstream copyFile(Helpers::editorDirectoryPath + std::string("assets/clipboard.kk"));
            Resources::Serializer::Write(copyFile, *io_selectedEntity);
            copyFile.close();
        }
    };
    auto pastFunc = [&io_entityManager, &io_selectedEntity](){
        if (io_selectedEntity)
        {
            std::ifstream copyFile(Helpers::editorDirectoryPath + std::string("assets/clipboard.kk"));

            if (copyFile.is_open())
            {
                Game::Entity* entity = io_entityManager.CreateEntity("newEntity");
                Resources::Serializer::ReadStandaloneEntity(copyFile, *entity);
            }
        }
    };

    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Copy", "CTRL+C"))
            copyFunc();
        if (ImGui::MenuItem("Paste", "CTRL+V"))
            pastFunc();

        ImGui::EndMenu();
    }

    if(Inputs::IsControlCommandPressed(EButton::C))
        copyFunc();
    if(Inputs::IsControlCommandPressed(EButton::V))
        pastFunc();
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

void Editor::MenuBar::GameField(Engine& io_engine, bool& o_loadScene)
{
    bool createComponent = false;

    bool isGaming = io_engine.GetRunMode() & Engine::RunFlag_Gaming;

    auto reloadSceneFunc = [&]()
            {
                io_engine.SetRunMode(Engine::RunFlag_Editing);
                io_engine.graph->ReloadScene();
                o_loadScene = true;
            };

    if (isGaming && Inputs::IsControlCommandPressed(EButton::P)) { reloadSceneFunc(); }

    if (ImGui::BeginMenu("Game"))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.9f, 0.4f, 1.0f)); // Yellow
        if (ImGui::MenuItem("Create component", "CTRL+G"))
            createComponent = true;
        ImGui::PopStyleColor();

        ImGui::Separator();

        if (ImGui::MenuItem("Play", "P")) { io_engine.SetRunMode(Engine::RunFlag_Editing | Engine::RunFlag_Gaming); }
        if (isGaming && ImGui::MenuItem("Pause", "P")) { io_engine.SetRunMode(Engine::RunFlag_Editing); }
        if (isGaming && ImGui::MenuItem("Stop", "CTRL+P"))  { reloadSceneFunc(); }

        ImGui::EndMenu();
    }

    if (createComponent || Inputs::IsControlCommandPressed(EButton::G))
        ImGui::OpenPopup("Create new component?");

    CreateComponentPopup();
}

void MenuBar::CreateComponentPopup()
{
    if (ImGui::BeginPopupModal("Create new component?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static std::string_view entryState = "none";

        static std::string compName = "NewComponent";
        Helpers::Edit(compName, "Name");                          // EDIT NAME

        if (ImGui::Button("Create", ImVec2(120, 0)))        // CREATE BUTTON
        {
            if (!Helpers::IsCamelCase(compName))                      // Bad name
                entryState = "Name not correct";
            else if (!CreateNewComponentScript(compName))       // Can't open file
                entryState = "File exists";
            else                                                            // Create new component
            {
                compName = "NewComponent";
                entryState = "none";

                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))        // CANCEL BUTTON
        {
            compName = "NewComponent";
            entryState = "none";
            ImGui::CloseCurrentPopup();
        }

        if (entryState == "File exists")                                    // WARNING POPUP LOG
            ImGui::Text("The file already exists or can't be located.");
        else if (entryState == "Name not correct")
            ImGui::Text("CamelCase convention not respected.");

        ImGui::EndPopup();
    }
}


void Editor::MenuBar::LightingField(Renderer::Graph& io_graph)
{
    if (ImGui::BeginMenu("Lighting"))
    {
        ImGui::Checkbox("Activate", &io_graph.lightEnabled);

        ImGui::Separator();

        Helpers::Edit(io_graph.light);

        ImGui::EndMenu();
    }
}
