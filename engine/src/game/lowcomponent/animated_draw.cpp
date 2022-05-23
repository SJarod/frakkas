#include <cassert>

#include "game/entity.hpp"
#include "game/time_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/light.hpp"
#include "resources/resources_manager.hpp"
#include "resources/skeletal_mesh.hpp"

#include "game/lowcomponent/animated_draw.hpp"

KK_COMPONENT_IMPL(AnimatedDraw)
KK_FIELD_IMPL(AnimatedDraw, skmodel, EDataType::SKELETALMODEL)

void AnimatedDraw::OnUpdate()
{
	skmodel.player.UpdatePlayer(Game::Time::GetDeltaTime());
}

void AnimatedDraw::Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform)
{
    if (!skmodel.skmesh)
        return;

    skmodel.UseShader();
    skmodel.SetUniform("uBoneTransforms", MAX_BONES, skmodel.player.GetBoneMatrices().data()->element);

	GLuint texToBeBinded = ResourcesManager::GetDefaultTexture().data;

	Resources::Texture* diffuseTex = skmodel.diffuseTex.get();
	if (diffuseTex != nullptr)
		if (diffuseTex->gpu.get())
			texToBeBinded = diffuseTex->gpu->data;

    for (std::shared_ptr<Submesh>& smesh : skmodel.skmesh->submeshes)
    {
        if (smesh == nullptr || smesh->gpu.VAO == 0)
            continue;

        Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetModelMatrix();
        skmodel.SetUniform("uModel", modelMat);
        Matrix4 modelNormal = (modelMat.Inverse()).Transpose();
        skmodel.SetUniform("uModelNormal", modelNormal);

        assert(smesh->gpu.VAO != 0);
        i_renderer.RenderMeshOnce(smesh->gpu.VAO,
            smesh->vertices.size(),
            texToBeBinded);

		if (i_renderer.outline)
		{
			skmodel.SetUniform("uOutline", true);
			i_renderer.RenderMeshOnceOutline(smesh->gpu.VAO, smesh->vertices.size());
			skmodel.SetUniform("uOutline", false);
		}
    }

    Resources::Shader::Unuse();
}

void AnimatedDraw::DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform)
{
	if (!skmodel.skmesh)
		return;

	skmodel.lightDepthShader->Use();
	skmodel.lightDepthShader->SetUniform("uBoneTransforms",
		MAX_BONES,
		skmodel.player.GetBoneMatrices().data()->element);

	for (std::shared_ptr<Submesh>& smesh : skmodel.skmesh->submeshes)
	{
		if (smesh == nullptr || smesh->gpu.VAO == 0)
			continue;

		Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetModelMatrix();
		skmodel.lightDepthShader->SetUniform("uModel", modelMat);
		assert(smesh->gpu.VAO != 0);
		i_renderer.RenderMeshOnce(smesh->gpu.VAO, smesh->vertices.size(), 0);
	}
    Shader::Unuse();
}
