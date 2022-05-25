#include <cassert>
#include <filesystem>

#include "game/entity.hpp"
#include "game/time_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/light.hpp"
#include "resources/resources_manager.hpp"
#include "resources/skeletal_mesh.hpp"

#include "utils/dragdrop_constants.hpp"

#include "game/lowcomponent/animated_draw.hpp"

KK_COMPONENT_IMPL_BEGIN(AnimatedDraw)
    KK_FIELD_PUSH(AnimatedDraw, meshPath, EDataType::STRING)
    KK_FIELD_CHANGECALLBACK(OnAnimationMeshPathUpdate)
    KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnAnimatedDrawComponent)

    KK_FIELD_PUSH(AnimatedDraw, texturePath, EDataType::STRING)
    KK_FIELD_CHANGECALLBACK(OnAnimationTexturePathUpdate)
    KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnAnimatedDrawComponent)

    KK_FIELD_PUSH(AnimatedDraw, animationPath, EDataType::STRING)
    KK_FIELD_CHANGECALLBACK(OnAnimationPathUpdate)
    KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, AnimationDropOnAnimatedDrawComponent)

    KK_FIELD_PUSH(AnimatedDraw, skmodel, EDataType::ANIMATION)

KK_COMPONENT_IMPL_END

void AnimatedDraw::OnUpdate()
{
	skmodel.player.UpdatePlayer(Game::Time::GetDeltaTime());
}

void AnimatedDraw::Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform)
{
    if (!skmodel.skmesh.lock())
        return;

    skmodel.UseShader();
    skmodel.SetUniform("uBoneTransforms", MAX_BONES, skmodel.player.GetBoneMatrices().data()->element);

	GLuint texToBeBinded = ResourcesManager::GetDefaultTexture().data;

	Resources::Texture* diffuseTex = skmodel.diffuseTex.lock().get();
	if (diffuseTex != nullptr)
		if (diffuseTex->gpu.get())
			texToBeBinded = diffuseTex->gpu->data;

    for (std::shared_ptr<Submesh>& smesh : skmodel.skmesh.lock()->submeshes)
    {
        if (smesh == nullptr || smesh->gpu.VAO == 0)
            continue;

        Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetWorldMatrix();
        skmodel.SetUniform("uModel", modelMat);
        Matrix4 modelNormal = (modelMat.Inverse()).Transpose();
        skmodel.SetUniform("uModelNormal", modelNormal);

        assert(smesh->gpu.VAO != 0);
        i_renderer.RenderMeshOnce(smesh->gpu.VAO,
            smesh->vertices.size(),
            texToBeBinded);

		if (i_renderer.outline)
			i_renderer.RenderMeshOnceOutline(smesh->gpu.VAO, smesh->vertices.size());
    }

    Resources::Shader::Unuse();
}

void AnimatedDraw::DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform)
{
	if (!skmodel.skmesh.lock())
		return;

	skmodel.lightDepthShader->Use();
	skmodel.lightDepthShader->SetUniform("uBoneTransforms",
		MAX_BONES,
		skmodel.player.GetBoneMatrices().data()->element);

	for (std::shared_ptr<Submesh>& smesh : skmodel.skmesh.lock()->submeshes)
	{
		if (smesh == nullptr || smesh->gpu.VAO == 0)
			continue;

		Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetWorldMatrix();
		skmodel.lightDepthShader->SetUniform("uModel", modelMat);
		assert(smesh->gpu.VAO != 0);
		i_renderer.RenderMeshOnce(smesh->gpu.VAO, smesh->vertices.size(), 0);
	}
    Shader::Unuse();
}

void AnimatedDraw::SetMesh(const std::string& i_path)
{
    meshPath = i_path;
    skmodel.SetSkeletalMeshFromFile(i_path);
}

void AnimatedDraw::SetTexture(const std::string& i_path, bool i_flip)
{
    texturePath = i_path;
    skmodel.SetTexture(i_path, i_flip);
}

void AnimatedDraw::SetAnimation(const std::string& i_path)
{
    animationPath = i_path;
    skmodel.LoadAnimationsForThis(i_path);
}

void DropOnAnimatedDrawComponent(unsigned char* io_component, void* io_dropData)
{
    auto& sd = *reinterpret_cast<AnimatedDraw*>(io_component);
    auto path = *reinterpret_cast<std::filesystem::path*>(io_dropData);
    std::string extension = path.extension().string();
    if (Utils::FindExtension(extension, Utils::MeshExtensions))
    {
        sd.meshPath = path.string();
        sd.skmodel.SetSkeletalMeshFromFile(sd.meshPath);
    }

    if (Utils::FindExtension(extension, Utils::TextureExtensions))
    {
        sd.texturePath = path.string();
        sd.skmodel.SetTexture(sd.texturePath, false);
    }
}

void AnimationDropOnAnimatedDrawComponent(unsigned char* io_component, void* io_dropData)
{
    auto& sd = *reinterpret_cast<AnimatedDraw*>(io_component);
    auto path = *reinterpret_cast<std::filesystem::path*>(io_dropData);
    std::string extension = path.extension().string();
    if (Utils::FindExtension(extension, Utils::AnimationExtensions))
    {
        sd.animationPath = path.string();
        sd.skmodel.LoadAnimationsForThis(sd.animationPath);
    }
}

void OnAnimationMeshPathUpdate(unsigned char* io_component)
{
    auto& sd = *reinterpret_cast<AnimatedDraw*>(io_component);
    sd.SetMesh(sd.meshPath);
}

void OnAnimationTexturePathUpdate(unsigned char* io_component)
{
    auto& sd = *reinterpret_cast<AnimatedDraw*>(io_component);
    sd.SetTexture(sd.texturePath, false);
}

void OnAnimationPathUpdate(unsigned char* io_component)
{
    auto& sd = *reinterpret_cast<AnimatedDraw*>(io_component);
    sd.SetAnimation(sd.animationPath);
}
