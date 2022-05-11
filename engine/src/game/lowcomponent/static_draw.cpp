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

    for (std::shared_ptr<Submesh>& smesh : model.mesh->submeshes)
    {
        if (smesh == nullptr || smesh->gpu.VAO == 0)
            continue;

        // outline
        i_renderer.SetUniformToNamedBlock("uRendering", 84, false);

        Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetModelMatrix();
        model.SetUniform("uModel", modelMat);
        Matrix4 modelNormal = (modelMat.Inverse()).Transpose();
        model.SetUniform("uModelNormal", modelNormal);

        GLuint texToBeBinded = ResourcesManager::GetDefaultTexture().data;

        Resources::Texture* diffuseTex = smesh->diffuseTex.get();
        if (diffuseTex != nullptr)
            if (diffuseTex->gpu.get())
                texToBeBinded = diffuseTex->gpu->data;

        assert(smesh->gpu.VAO != 0);
        i_renderer.RenderMeshOnce(smesh->gpu.VAO,
            smesh->vertices.size(),
            texToBeBinded);

        if (i_light.outline)
        {
            // outline
            i_renderer.SetUniformToNamedBlock("uRendering", 84, true);
            i_renderer.RenderMeshOnceOutline(smesh->gpu.VAO, smesh->vertices.size(), i_light.outlineSize);
        }
    }
}

void StaticDraw::OnUpdate()
{
    auto& descriptors = GetMetaData().descriptors;
    auto it = std::find_if(descriptors.begin(), descriptors.end(), [](const DataDescriptor& desc)
    {
        return "meshPath" == desc.name;
    });
    if (it != descriptors.end())
    {
    }
}
