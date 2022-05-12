#include <cassert>

#include "resources/mesh.hpp"
#include "resources/resources_manager.hpp"

#include "resources/resources_manager.hpp"

#include "renderer/model.hpp"

Renderer::Model::Model()
	: RenderObject("basicShader", "engine/shaders/basic")
{
	lightDepthShader = ResourcesManager::LoadResource<Resources::Shader>("depthmapShader", "engine/shaders/light_depth");
}

Renderer::Model::Model(const std::string& i_meshFilename)
	: RenderObject("basicShader", "engine/shaders/basic")
{
	lightDepthShader = ResourcesManager::LoadResource<Resources::Shader>("depthmapShader", "engine/shaders/light_depth");
	mesh = ResourcesManager::LoadResource<Mesh>(i_meshFilename);
}

Renderer::Model::Model(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
	: RenderObject("basicShader", "engine/shaders/basic")
{
	lightDepthShader = ResourcesManager::LoadResource<Resources::Shader>("depthmapShader", "engine/shaders/light_depth");
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

void Renderer::Model::SetTextureToSubmesh(const std::string& i_textureFilename, const bool i_flipTexture, const unsigned int i_meshIndex) const
{
	assert(i_meshIndex < mesh->submeshes.size() && "out of range");

	Submesh* smesh = std::next(mesh->submeshes.begin(), i_meshIndex)->get();
	smesh->diffuseTex = ResourcesManager::LoadResource<Texture>(i_textureFilename, i_flipTexture);
}

void Renderer::Model::SetTextureToAllSubmesh(const std::string& i_textureFilename, const bool i_flipTexture) const
{
    if (!mesh)
        return;

    mesh->flipTexture = i_flipTexture;
    mesh->textureName = i_textureFilename;

    for (int i = 0; i < static_cast<int>(mesh->submeshes.size()); i++)
        SetTextureToSubmesh(i_textureFilename, i_flipTexture, i);
}
