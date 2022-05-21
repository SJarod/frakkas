#include <imgui.h>

#include "debug/log.hpp"

#include "game/inputs_manager.hpp"

#include "editor/file_browser.hpp"

using namespace Editor;

extern const std::filesystem::path g_assetPath = "game/assets";

FileBrowser::FileBrowser() : m_currentDirectory(g_assetPath),
    m_directoryIcon("editor/assets/DirectoryIcon.png", true),
    m_fileIcon("editor/assets/FileIcon.png", true),
    m_wavIcon("editor/assets/WAVicon.png", true),
    m_mp3Icon("editor/assets/MP3icon.png", true),
    m_objIcon("editor/assets/OBJicon.png", true),
    m_fbxIcon("editor/assets/FBXicon.png", true),
    m_gltfIcon("editor/assets/GLTFicon.png", true),
    m_pngIcon("editor/assets/PNGicon.png", true),
    m_jpgIcon("editor/assets/JPGicon.png", true),
    m_kkIcon("editor/assets/KKicon.png", true),
    m_cppIcon("editor/assets/CPPicon.png", true),
    m_hppIcon("editor/assets/HPPicon.png", true)
{
    m_directoryIcon.LoadFromInfo();
    m_fileIcon.LoadFromInfo();
    m_wavIcon.LoadFromInfo();
    m_mp3Icon.LoadFromInfo();
    m_objIcon.LoadFromInfo();
    m_fbxIcon.LoadFromInfo();
    m_gltfIcon.LoadFromInfo();
    m_pngIcon.LoadFromInfo();
    m_jpgIcon.LoadFromInfo();
    m_kkIcon.LoadFromInfo();
    m_cppIcon.LoadFromInfo();
    m_hppIcon.LoadFromInfo();
}

void FileBrowser::OnImGuiRender()
{
	// Create assets directory if it doesn't exist
	if (!std::filesystem::is_directory(m_currentDirectory) || std::filesystem::exists(m_currentDirectory))
		std::filesystem::create_directory(m_currentDirectory);

	ImGui::Begin("File Browser");

	// Create back button to go up the assets directory hierarchy
	if (m_currentDirectory != std::filesystem::path(g_assetPath))
	{
		if (ImGui::Button("<- back"))
			m_currentDirectory = m_currentDirectory.parent_path();
	}

	// Change buttons size and padding
	static float padding = 16.f;
	static float thumbnailSize = 70.f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);

	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, nullptr, false);

    bool openDeletePopup = false;
	// Navigate through assets directory
	for (const auto& entry : std::filesystem::directory_iterator(m_currentDirectory))
    {
        std::filesystem::path path = entry.path();
        path.make_preferred();
        std::string filenameString = entry.path().filename().string();

        ImGui::PushID(filenameString.c_str());

        // Add button texture
        Resources::Texture& icon = ButtonIcon(entry);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        if (icon.gpu)
        {
            ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon.gpu->data), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});
            if(ImGui::IsItemHovered() && Game::Inputs::IsPressed(Game::EButton::MOUSE_RIGHT))
            {
                m_deletePath = entry.path();
                openDeletePopup = true;
            }
        }

        if (ImGui::BeginDragDropSource())
        {
            static std::filesystem::path itemPath;
            itemPath = entry.path();
            ImGui::SetDragDropPayload("FILE_BROWSER_ITEM", &itemPath, sizeof(std::filesystem::path));
            ImGui::EndDragDropSource();
        }

        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (entry.is_directory())
                m_currentDirectory /= path.filename();
            else
            {
                std::string openPath = "\"" + path.string() + "\"";
                std::system(openPath.c_str());
            }
        }
        ImGui::TextWrapped("%s", filenameString.c_str());

        ImGui::NextColumn();

        ImGui::PopID();
	}

	ImGui::Columns(1);

	OptionsField(&padding, &thumbnailSize);

	ImGui::End();

    if (openDeletePopup)
        ImGui::OpenPopup("DELETE");

    if (ImGui::BeginPopup("DELETE", ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::Selectable("Delete"))
        {
            if (!std::filesystem::remove(m_deletePath))
                Log::Error("Can't delete file/directory '", m_deletePath.string(), "'.");
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void FileBrowser::OptionsField(float* io_padding, float* io_thumbnailSize)
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Options"))
	{
		if (ImGui::BeginMenu("File Browser"))
		{
			ImGui::SliderFloat("Thumbnail Size", io_thumbnailSize, 50, 160);
			ImGui::SliderFloat("Padding", io_padding, 0, 64);

			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

Resources::Texture& FileBrowser::ButtonIcon(const std::filesystem::directory_entry& entry)
{
    if (entry.is_directory())
        return m_directoryIcon;
    else if (entry.path().extension() == ".wav")
        return m_wavIcon;
    else if (entry.path().extension() == ".mp3")
        return m_mp3Icon;
    else if (entry.path().extension() == ".obj")
        return m_objIcon;
    else if (entry.path().extension() == ".fbx")
        return m_fbxIcon;
    else if (entry.path().extension() == ".gltf")
        return m_gltfIcon;
    else if (entry.path().extension() == ".png")
        return m_pngIcon;
    else if (entry.path().extension() == ".jpg")
        return m_jpgIcon;
    else if (entry.path().extension() == ".kk")
        return m_kkIcon;
    else if (entry.path().extension() == ".cpp")
        return m_cppIcon;
    else if (entry.path().extension() == ".hpp")
        return m_hppIcon;
    else
        return m_fileIcon;
}
