#include <imgui.h>

#include "maths.hpp"
#include "debug/log.hpp"

#include "game/time_manager.hpp"
#include "game/inputs_manager.hpp"
#include "game/entity_manager.hpp"

#include "renderer/graph.hpp"

#include "editor/debugger.hpp"
#include "engine.hpp"

using namespace Editor;

void Debugger::OnImGuiRender(Renderer::Graph& io_graph, bool& o_gaming, bool& o_reloadScene)
{
    ImGui::Begin("Debugger");

#pragma region Useful button
    if(ImGui::Button("Save scene"))
        io_graph.SaveScene();

    if (ImGui::Button("Load scene"))
    {
        io_graph.LoadScene("exemple_scene");
        o_reloadScene = true;
    }
    
    if(ImGui::Button("Play") || Game::Inputs::IsPressed("pause"))
        o_gaming = !o_gaming;

#pragma endregion


#pragma region Performance
    float newTime = Game::Time::GetTime();
    float newDeltaTime = Game::Time::GetDeltaTime();
    int newFPS = round(1.f / newDeltaTime);
    // Update frame performance value sometimes so that we can read the value correctly.
    if (newTime - prevTime >= timeBeforeUpdate)
    {
        prevTime = newTime;
        deltaTime = newDeltaTime;
        FPS = newFPS;
    }

    ImGui::Text("Time : %.2f", newTime);

    float timeScale = Game::Time::GetTimeScale();
    ImGui::InputFloat("Time scale", &timeScale, 0.1f, 0.2f, "%.2f");
    timeScale = Maths::Clamp(timeScale, 0.f, 3.f);
    Game::Time::SetTimeScale(timeScale);

    ImGui::Text("Fixed Delta time : %.4f", Game::Time::GetFixedDeltaTime());
    ImGui::SameLine();
    ImGui::Text("Delta time : %.4f", deltaTime);

    ImGui::Spacing();

    ImGui::Text("FPS : %i", FPS);
#pragma endregion
    ImGui::End();
}
