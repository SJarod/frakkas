#pragma once

namespace Renderer
{
    class Graph;
    class Light;
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
        void EditField();
        void OptionsField();
        /**
         * @brief Give useful utilities about Game experience.
         * @param o_gaming The current gaming state (playing or not playing)
         */
        void GameField(bool& o_gaming);
        /**
         * @brief Edit lighting
         * @param io_graph The graph which possesses the light.
         */
        void LightingField(Renderer::Graph& io_graph);
    };
}
