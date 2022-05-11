#include <cassert>

#include "debug/log.hpp"
#include "resources/resources_manager.hpp"

#include "renderer/skeletal_model.hpp"

Renderer::SkeletalModel::SkeletalModel(const std::string& i_skmeshFilename)
	: RenderObject("engine/shaders/animation", "engine/shaders/basic")
{
	skmesh = ResourcesManager::LoadResource<Resources::SkeletalMesh>(i_skmeshFilename);
}

Renderer::SkeletalModel::SkeletalModel(const std::string& i_skmeshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
	: RenderObject("engine/shaders/animation", "engine/shaders/basic")
{
	skmesh = ResourcesManager::LoadResource<Resources::SkeletalMesh>(i_skmeshFilename, i_textureFilename, i_flipTexture);
}

void Renderer::SkeletalModel::SetSkeletalMeshFromFile(const std::string& i_skmeshFilename)
{
	skmesh = ResourcesManager::LoadResource<Resources::SkeletalMesh>(i_skmeshFilename);
}

void Renderer::SkeletalModel::SetSkeletalMeshFromFile(const std::string& i_skmeshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
{
	skmesh = ResourcesManager::LoadResource<Resources::SkeletalMesh>(i_skmeshFilename, i_textureFilename, i_flipTexture);
}

void Renderer::SkeletalModel::SetTextureToSubmesh(const std::string& i_textureFilename, const bool i_flipTexture, const unsigned int i_skmeshIndex) const
{
	assert(i_skmeshIndex < skmesh->submeshes.size() && "out of range");

	Submesh* smesh = std::next(skmesh->submeshes.begin(), i_skmeshIndex)->get();
	smesh->diffuseTex = ResourcesManager::LoadResource<Texture>(i_textureFilename, i_flipTexture);
}

void Renderer::SkeletalModel::SetTextureToAllSubmesh(const std::string& i_textureFilename, const bool i_flipTexture) const
{
	skmesh->flipTexture = i_flipTexture;
	skmesh->textureName = i_textureFilename;

	for (int i = 0; i < static_cast<int>(skmesh->submeshes.size()); i++)
		SetTextureToSubmesh(i_textureFilename, i_flipTexture, i);
}

void Renderer::SkeletalModel::LoadAnimationsForThis(const std::string& i_animationFilename)
{
	if (!skmesh)
	{
		Log::Info("No skeletal mesh to attach animation to");
		return;
	}

	skpack = ResourcesManager::LoadResource<SkeletalAnimationPack>(i_animationFilename + "|" + skmesh->name, i_animationFilename, std::ref(*skmesh));
}