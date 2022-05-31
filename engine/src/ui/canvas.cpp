#include "ui/canvas.hpp"

using namespace UI;

Canvas::Canvas()
{
    windowSettings = ImGuiWindowFlags_NoTitleBar;
    windowSettings |= ImGuiWindowFlags_NoBackground;
    windowSettings |= ImGuiWindowFlags_NoDecoration;
    windowSettings |= ImGuiWindowFlags_NoCollapse;
    windowSettings |= ImGuiWindowFlags_NoDocking;
    windowSettings |= ImGuiWindowFlags_NoFocusOnAppearing;
    windowSettings |= ImGuiWindowFlags_NoMove;
    windowSettings |= ImGuiWindowFlags_NoSavedSettings;
}

void Canvas::AddUIObject(Game::UIObject* io_uiObject)
{
    io_uiObject->id = maxID++;
    objects.emplace_back(io_uiObject);
}

void Canvas::RemoveUIObject(const Game::UIObject* i_uiObject)
{
    auto it = std::find_if(objects.begin(), objects.end(), [&i_uiObject](Game::UIObject* object)
    {
        return i_uiObject->id == object->id;
    });

    if (it != objects.end())
        objects.erase(it);
}

void Canvas::Clear()
{
    objects.clear();
}

void Canvas::Render(const Vector2& i_offset) const
{
    ImDrawList& drawList = *ImGui::GetWindowDrawList();

    drawList.AddCircleFilled(ImVec2(20.f, 20.f), 0.5f, ImColor(255, 0, 0));

    Vector2 origin(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    origin = origin + i_offset;
    Vector2 size(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
    size = size - i_offset * 2.f;

    for(Game::UIObject* uiObject : objects)
        if (uiObject->enabled)
            uiObject->UpdateAndRender(drawList, origin, size);

    ImGui::SetWindowFontScale(1.f);
}

void Canvas::BeginAndRender() const
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowFocus();

    ImGui::Begin("Canvas", nullptr, windowSettings);

    Render();

    ImGui::End();
}
