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

        Resources::Texture m_directoryIcon;
        Resources::Texture m_fileIcon;

        Resources::Texture m_wavIcon;
        Resources::Texture m_mp3Icon;
        Resources::Texture m_objIcon;
        Resources::Texture m_fbxIcon;
        Resources::Texture m_gltfIcon;
        Resources::Texture m_pngIcon;
        Resources::Texture m_jpgIcon;
        Resources::Texture m_kkIcon;
        Resources::Texture m_cppIcon;
        Resources::Texture m_hppIcon;

        /**
        * @summary Add "Options" field to Main menu bar to modify buttons size and padding
        * @param padding the space between each icon
        * @param thumbnailSize the icon's size
        */
        void OptionsField(float* padding, float* thumbnailSize);

        /**
        * @summary Change button texture depending file extension
        */
        Resources::Texture& ButtonIcon(const std::filesystem::directory_entry& entry);
    };
}
