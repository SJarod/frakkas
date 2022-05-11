#include <imgui.h>

#include "resources/resources_manager.hpp"

#include "editor/file_browser.hpp"


using namespace Editor;


extern const std::filesystem::path g_assetPath = "game/assets";

FileBrowser::FileBrowser() : m_currentDirectory(g_assetPath)
{
    m_directoryIcon = ResourcesManager::LoadResource<Resources::Texture>("editor/assets/DirectoryIcon.png", true);
    m_fileIcon = ResourcesManager::LoadResource<Resources::Texture>("editor/assets/FileIcon.png", true);
    m_wavIcon = ResourcesManager::LoadResource<Resources::Texture>("editor/assets/WAVicon.png", true);
    m_mp3Icon = ResourcesManager::LoadResource<Resources::Texture>("editor/assets/MP3icon.png", true);
    m_objIcon = ResourcesManager::LoadResource<Resources::Texture>("editor/assets/OBJicon.png", true);
    m_fbxIcon = ResourcesManager::LoadResource<Resources::Texture>("editor/assets/FBXicon.png", true);
    m_gltfIcon = ResourcesManager::LoadResource<Resources::Texture>("editor/assets/GLTFicon.png", true);
    m_pngIcon = ResourcesManager::LoadResource<Resources::Texture>("editor/assets/PNGicon.png", true);
    m_jpgIcon = ResourcesManager::LoadResource<Resources::Texture>("editor/assets/JPGicon.png", true);
    m_kkIcon = ResourcesManager::LoadResource<Resources::Texture>("editor/assets/KKicon.png", true);
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

	ImGui::Columns(columnCount, 0, false);

	// Navigate through assets directory
	for (const auto& entry : std::filesystem::directory_iterator(m_currentDirectory))
    {
        const std::filesystem::path& path = entry.path();
        std::string filenameString = entry.path().filename().string();

        ImGui::PushID(filenameString.c_str());

        // Add button texture
        std::shared_ptr<Resources::Texture> icon = ButtonIcon(entry);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        if (icon && icon->gpu)
            ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->gpu->data), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});

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
        }
        ImGui::TextWrapped("%s", filenameString.c_str());

        ImGui::NextColumn();

        ImGui::PopID();
	}

	ImGui::Columns(1);

	OptionsField(&padding, &thumbnailSize);

	ImGui::End();
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

std::shared_ptr<Resources::Texture> FileBrowser::ButtonIcon(const std::filesystem::directory_entry& entry)
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
    else
        return m_fileIcon;
}