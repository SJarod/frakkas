#include <imgui.h>

#include "resources/resources_manager.hpp"

#include "editor/file_browser.hpp"


using namespace Editor;


extern const std::filesystem::path g_assetPath = "game/assets";

FileBrowser::FileBrowser() : m_currentDirectory(g_assetPath)
{
	m_directoryIcon = Resources::ResourcesManager::LoadResource<Texture>("editor/assets/DirectoryIcon.png", true);
	m_fileIcon = Resources::ResourcesManager::LoadResource<Texture>("editor/assets/FileIcon.png", true);
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
	static float thumbnailSize = 128.f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);

	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	// Navigate through assets directory
	for (const auto& entry : std::filesystem::directory_iterator(m_currentDirectory)) {
		const auto& path = entry.path();
		auto relativePath = std::filesystem::relative(path, g_assetPath);
		std::string filenameString = relativePath.filename().string();

		ImGui::PushID(filenameString.c_str());

		// Add button texture
		std::shared_ptr<Resources::Texture> icon = entry.is_directory() ? m_directoryIcon : m_fileIcon;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (icon && icon->gpu)
			ImGui::ImageButton((ImTextureID)icon->gpu->data, { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
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
