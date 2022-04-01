#pragma once

namespace Editor
{
    class Console
    {
    public:
        Console() = default;
        ~Console() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();

    private:
        ImVector<char*>       items;
        ImGuiTextFilter       filter;
        bool                  autoScroll = true;

        void AddLog(const char* i_fmt, ...);
        void ClearLog();
        void DisplayLogList();
    };
}
