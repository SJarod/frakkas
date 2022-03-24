//
// Created by manon on 17/03/2022.
//

#pragma once

#include <filesystem>

#include "resources/texture.hpp"

namespace Editor
{
    class FileBrowser
    {
    public:
        FileBrowser();
        ~FileBrowser() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();

    private:
        std::filesystem::path m_currentDirectory;

        std::shared_ptr<Resources::Texture> m_directoryIcon;
        std::shared_ptr<Resources::Texture> m_fileIcon;

        /**
        * @summary Add "Options" field to Main menu bar to modify buttons size and padding
        */
        void OptionsField(float* padding, float* thumbnailSize);
    };
}