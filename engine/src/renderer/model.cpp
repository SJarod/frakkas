#include <cassert>

#include "resources/mesh.hpp"
#include "resources/resources_manager.hpp"

#include "renderer/model.hpp"

Renderer::Model::Model(const std::string& i_meshFilename)
{
	//meshes.reserve(10000);
	ResourcesManager::LoadModel(meshes, i_meshFilename);
}

Renderer::Model::Model(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
{
	ResourcesManager::LoadModel(meshes, i_meshFilename, i_textureFilename, i_flipTexture);
}

void Renderer::Model::AddCubeMesh()
{
	ResourcesManager::LoadCube(meshes);
}

void Renderer::Model::AddSphereMesh(const float i_radius, const int i_longitude, const int i_latitude)
{
	ResourcesManager::LoadSphere(meshes, i_radius, i_longitude, i_latitude);
}

void Renderer::Model::AddMeshesFromFile(const std::string& i_meshFilename)
{
	ResourcesManager::LoadModel(meshes, i_meshFilename);
}

void Renderer::Model::AddMeshesFromFile(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
{
	ResourcesManager::LoadModel(meshes, i_meshFilename, i_textureFilename, i_flipTexture);
}

void Renderer::Model::AddTextureToMesh(const std::string& i_textureFilename, const bool i_flipTexture, const unsigned int i_meshIndex)
{
	assert(i_meshIndex < meshes.size() && "out of range");

	Mesh* mesh = std::next(meshes.begin(), i_meshIndex)->get();
	mesh->diffuseTex = ResourcesManager::LoadResource<Texture>(i_textureFilename, i_flipTexture);
}
