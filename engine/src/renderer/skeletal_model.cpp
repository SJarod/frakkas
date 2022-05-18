#include <cassert>

#include "debug/log.hpp"
#include "resources/resources_manager.hpp"

#include "renderer/skeletal_model.hpp"

Renderer::SkeletalModel::SkeletalModel()
	: RenderObject("basicShader + defineSKINNING", "engine/shaders/basic", { "#define SKINNING\n" })
{
	const std::initializer_list<const std::string> defines = { "#define SKINNING\n" };
	lightDepthShader = ResourcesManager::LoadResource<Resources::Shader>("depthmapShaderAnim",
		"engine/shaders/light_depth", defines);
}

Renderer::SkeletalModel::SkeletalModel(const std::string& i_skmeshFilename)
	: RenderObject("basicShader + defineSKINNING", "engine/shaders/basic", { "#define SKINNING\n" })
{
	const std::initializer_list<const std::string> defines = { "#define SKINNING\n" };
	lightDepthShader = ResourcesManager::LoadResource<Resources::Shader>("depthmapShaderAnim",
		"engine/shaders/light_depth", defines);
	skmesh = ResourcesManager::LoadResource<Resources::SkeletalMesh>(i_skmeshFilename);
}

Renderer::SkeletalModel::SkeletalModel(const std::string& i_skmeshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
	: RenderObject("basicShader + defineSKINNING", "engine/shaders/basic", { "#define SKINNING\n" })
{
	const std::initializer_list<const std::string> defines = { "#define SKINNING\n" };
	lightDepthShader = ResourcesManager::LoadResource<Resources::Shader>("depthmapShaderAnim",
		"engine/shaders/light_depth", defines);
	skmesh = ResourcesManager::LoadResource<Resources::SkeletalMesh>(i_skmeshFilename);
	SetTexture(i_textureFilename, i_flipTexture);
}

void Renderer::SkeletalModel::SetSkeletalMeshFromFile(const std::string& i_skmeshFilename)
{
	skmesh = ResourcesManager::LoadResource<Resources::SkeletalMesh>(i_skmeshFilename);
}

void Renderer::SkeletalModel::SetSkeletalMeshFromFile(const std::string& i_skmeshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
{
	skmesh = ResourcesManager::LoadResource<Resources::SkeletalMesh>(i_skmeshFilename);
	SetTexture(i_textureFilename, i_flipTexture);
}

void Renderer::SkeletalModel::SetTexture(const std::string& i_textureFilename, const bool i_flipTexture)
{
    if (!skmesh)
        return;

	flipTexture = i_flipTexture;
	textureName = i_textureFilename;

	diffuseTex = ResourcesManager::LoadResource<Texture>(i_textureFilename, i_flipTexture);
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