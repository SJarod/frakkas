#include <fstream>
#include <Tracy.hpp>

#include "debug/log.hpp"

#include "editor/console.hpp"


using namespace Editor;


void Console::OnImGuiRender()
{
    ImGui::Begin("Console");

    DisplayLogList();

    if (ImGui::IsWindowAppearing())
        scrollToBottom = true;

    if (ImGui::Button("AddLog"))
        Log::Info("Add Log");
    ImGui::SameLine();

    // clear console
    if (ImGui::Button("Clear"))
        ClearLog();
    ImGui::SameLine();

    // auto-scroll menu
    ImGui::Checkbox("Auto-scroll", &autoScroll);
    ImGui::SameLine();

    // Filter bar
    filter.Draw("Filter (\".h\") (\"error\")", 180);
    ImGui::Separator();

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

    // Change log color
    for (int i = 0; i < items.Size; i++)
    {
        const char* item = items[i];
        if (!filter.PassFilter(item))
            continue;

        ImVec4 color;
        bool has_color = false;
        if (strstr(item, "[WARNING]"))
        {
            color = ImVec4(1.0f, 0.9f, 0.4f, 1.0f); // yellow
            has_color = true;
        }
        else if (strstr(item, "[ERROR]"))
        {
            color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); // red
            has_color = true;
        }
        if (has_color)
            ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(item);
        if (has_color)
            ImGui::PopStyleColor();
    }

    if (scrollToBottom || (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);

    scrollToBottom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();

    ImGui::End();
}

void Console::AddLog(const char* i_fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, i_fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), i_fmt, args);
    buf[IM_ARRAYSIZE(buf)-1] = 0;
    va_end(args);
    items.push_back(strdup(buf));

    if (autoScroll == true)
        scrollToBottom = true;
}

void Console::ClearLog()
{
    for (int i = 0; i < items.Size; i++)
        free(items[i]);
    items.clear();
}

void Console::DisplayLogList()
{
    std::vector<std::string>& listLogs = Log::GetLogList();

    if (!listLogs.empty())
    {
        for (int i = 0; i < listLogs.size(); i++)
            AddLog("%s", listLogs[i].c_str());

        listLogs.clear();
    }
}
