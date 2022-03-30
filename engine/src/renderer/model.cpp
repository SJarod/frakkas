#include <cassert>

#include <glad/glad.h>
#include <imgui.h>

#include "resources/mesh.hpp"

#include "resources/resources_manager.hpp"

#include "renderer/model.hpp"

Renderer::Model::Model(const std::string& i_meshFilename)
{
	meshes = ResourcesManager::LoadModel(i_meshFilename, true);
}

Renderer::Model::Model(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
{
	meshes = ResourcesManager::LoadModel(i_meshFilename, false);
	std::shared_ptr<Texture> tex = ResourcesManager::LoadTexture(i_textureFilename, i_flipTexture);

	for (int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].get()->diffuseTex = tex.get()->gpu;
	}
}

void Renderer::Model::AddCubeMesh()
{
	meshes.push_back(ResourcesManager::LoadCube());
}

void Renderer::Model::AddSphereMesh(const float i_radius, const int i_longitude, const int i_latitude)
{
	meshes.push_back(ResourcesManager::LoadSphere(i_radius, i_longitude, i_latitude));
}

void Renderer::Model::AddMeshesFromFile(const std::string& i_meshFilename)
{
	std::vector<std::shared_ptr<Resources::Mesh>> loaded = ResourcesManager::LoadModel(i_meshFilename, true);

	for (const auto& mesh : loaded)
	{
		meshes.push_back(mesh);
	}
}

void Renderer::Model::AddMeshesFromFile(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
{
	std::vector<std::shared_ptr<Resources::Mesh>> loaded = ResourcesManager::LoadModel(i_meshFilename, false);

	for (const auto& mesh : loaded)
	{
		meshes.push_back(mesh);
	}

	std::shared_ptr<Texture> tex = ResourcesManager::LoadTexture(i_textureFilename, i_flipTexture);

	for (int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].get()->diffuseTex = tex.get()->gpu;
	}
}

void Renderer::Model::AddTextureToMesh(const std::string& i_textureFilename, const bool i_flipTexture, const unsigned int i_meshIndex)
{
	assert(i_meshIndex < meshes.size() && "out of range");

	std::shared_ptr<Texture> tex = ResourcesManager::LoadTexture(i_textureFilename, i_flipTexture);

	meshes[i_meshIndex].get()->diffuseTex = tex.get()->gpu;
}

void Renderer::Model::Edit() {

    ImGui::Text("Model");
    ImGui::Separator();
    transform.Edit();

}
