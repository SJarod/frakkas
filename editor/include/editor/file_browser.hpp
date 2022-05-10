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

        std::shared_ptr<Resources::Texture> m_wavIcon;
        std::shared_ptr<Resources::Texture> m_mp3Icon;
        std::shared_ptr<Resources::Texture> m_objIcon;
        std::shared_ptr<Resources::Texture> m_fbxIcon;
        std::shared_ptr<Resources::Texture> m_gltfIcon;
        std::shared_ptr<Resources::Texture> m_pngIcon;
        std::shared_ptr<Resources::Texture> m_jpgIcon;
        std::shared_ptr<Resources::Texture> m_kkIcon;

        /**
        * @summary Add "Options" field to Main menu bar to modify buttons size and padding
        * @param padding the space between each icon
        * @param thumbnailSize the icon's size
        */
        void OptionsField(float* padding, float* thumbnailSize);

        /**
        * @summary Change button texture depending file extension
        */
        std::shared_ptr<Resources::Texture> ButtonIcon(const std::filesystem::directory_entry& entry);
    };
}
