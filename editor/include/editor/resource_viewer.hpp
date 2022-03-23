//
// Created by manon on 17/03/2022.
//

#pragma once

namespace Editor
{
    class ResourceViewer
    {
    public:
        ResourceViewer() = default;
        ~ResourceViewer() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();
    };
}