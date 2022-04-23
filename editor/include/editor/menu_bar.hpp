#pragma once

namespace Renderer
{
    class Graph;
}

namespace Editor
{
    class MenuBar
    {
    public:
        MenuBar() = default;
        ~MenuBar() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender(Renderer::Graph& io_graph, bool& o_gaming, bool& o_loadScene);
        void FrakkasColors();

    private:
        /**
         * @brief Build the imgui interface of scene and project file utility.
         * @param io_graph The graph to manage scene from.
         */
        void FileField(Renderer::Graph& io_graph, bool& o_loadScene);
        void EditField(bool& o_gaming);
        void OptionsField();
    };
}
