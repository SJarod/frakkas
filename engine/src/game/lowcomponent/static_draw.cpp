#include <cassert>

#include "game/entity.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/light.hpp"
#include "resources/resources_manager.hpp"
#include "resources/mesh.hpp"

#include "game/lowcomponent/static_draw.hpp"

KK_COMPONENT_IMPL(StaticDraw)
KK_FIELD_IMPL(StaticDraw, model, EDataType::MODEL)

void StaticDraw::Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform)
{
    if (!model.mesh)
        return;

    model.UseShader();

    GLuint texToBeBinded = ResourcesManager::GetDefaultTexture().data;

    Resources::Texture* diffuseTex = model.diffuseTex.get();
    if (diffuseTex != nullptr)
        if (diffuseTex->gpu.get())
            texToBeBinded = diffuseTex->gpu->data;

    for (std::shared_ptr<Submesh>& smesh : model.mesh->submeshes)
    {
        if (smesh == nullptr || smesh->gpu.VAO == 0)
            continue;

        Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetModelMatrix();
        model.SetUniform("uModel", modelMat);
        Matrix4 modelNormal = (modelMat.Inverse()).Transpose();
        model.SetUniform("uModelNormal", modelNormal);

        assert(smesh->gpu.VAO != 0);
        i_renderer.RenderMeshOnce(smesh->gpu.VAO,
            smesh->vertices.size(),
            texToBeBinded);

		if (i_renderer.outline)
		{
			model.SetUniform("uOutline", true);
			i_renderer.RenderMeshOnceOutline(smesh->gpu.VAO, smesh->vertices.size());
			model.SetUniform("uOutline", false);
		}
    }
}

void StaticDraw::DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform)
{
	if (!model.mesh)
		return;

	model.lightDepthShader->Use();

	for (std::shared_ptr<Submesh>& smesh : model.mesh->submeshes)
	{
		if (smesh == nullptr || smesh->gpu.VAO == 0)
			continue;

		Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetModelMatrix();
		model.lightDepthShader->SetUniform("uModel", modelMat);
		assert(smesh->gpu.VAO != 0);
		i_renderer.RenderMeshOnce(smesh->gpu.VAO, smesh->vertices.size(), 0);
	}
}
