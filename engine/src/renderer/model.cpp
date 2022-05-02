#include <cassert>

#include "resources/mesh.hpp"
#include "resources/resources_manager.hpp"

#include "renderer/model.hpp"

Renderer::Model::Model(const std::string& i_meshFilename)
	: RenderObject("engine/shaders/basic")
{
	mesh = ResourcesManager::LoadResource<Mesh>(i_meshFilename);
}

Renderer::Model::Model(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
	: RenderObject("engine/shaders/basic")
{
	mesh = ResourcesManager::LoadResource<Mesh>(i_meshFilename, i_textureFilename, i_flipTexture);
}

void Renderer::Model::SetMeshFromFile(const std::string& i_meshFilename)
{
	mesh = ResourcesManager::LoadResource<Mesh>(i_meshFilename);
}

void Renderer::Model::SetMeshFromFile(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
{
	mesh = ResourcesManager::LoadResource<Mesh>(i_meshFilename, i_textureFilename, i_flipTexture);
}

void Renderer::Model::SetTextureToSubmesh(const std::string& i_textureFilename, const bool i_flipTexture, const unsigned int i_meshIndex)
{
	assert(i_meshIndex < mesh->submeshes.size() && "out of range");

	Submesh* smesh = std::next(mesh->submeshes.begin(), i_meshIndex)->get();
	smesh->diffuseTex = ResourcesManager::LoadResource<Texture>(i_textureFilename, i_flipTexture);
}