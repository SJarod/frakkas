#include <cassert>
#include <filesystem>

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/light.hpp"
#include "resources/resources_manager.hpp"
#include "resources/mesh.hpp"

#include "utils/dragdrop_constants.hpp"

#include "game/entity.hpp"
#include "game/lowcomponent/static_draw.hpp"


KK_COMPONENT_IMPL_BEGIN(StaticDraw)
    KK_FIELD_PUSH(StaticDraw, meshPath, EDataType::STRING)
    KK_FIELD_CHANGECALLBACK(OnMeshPathUpdate)
    KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnStaticDrawComponent)

    KK_FIELD_PUSH(StaticDraw, texturePath, EDataType::STRING)
    KK_FIELD_CHANGECALLBACK(OnTexturePathUpdate)
    KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnStaticDrawComponent)

KK_COMPONENT_IMPL_END

void StaticDraw::Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform)
{
    if (model.mesh.expired() || !model.mesh.lock())
        return;

    model.UseShader();

    GLuint texToBeBinded = ResourcesManager::GetDefaultTexture().data;

    Resources::Texture* diffuseTex = model.diffuseTex.lock().get();
    if (diffuseTex != nullptr)
        if (diffuseTex->gpu.get())
            texToBeBinded = diffuseTex->gpu->data;

    for (std::shared_ptr<Submesh>& smesh : model.mesh.lock()->submeshes)
    {
        if (smesh == nullptr || smesh->gpu.VAO == 0)
            continue;

        Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetWorldMatrix();
        model.SetUniform("uModel", modelMat);
        Matrix4 modelNormal = (modelMat.Inverse()).Transpose();
        model.SetUniform("uModelNormal", modelNormal);

        assert(smesh->gpu.VAO != 0);
        i_renderer.RenderMeshOnce(smesh->gpu.VAO,
            smesh->vertices.size(),
            texToBeBinded);

		if (i_renderer.outline)
			i_renderer.RenderMeshOnceOutline(smesh->gpu.VAO, smesh->vertices.size());
    }
    Resources::Shader::Unuse();
}

void StaticDraw::DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform)
{
	if (model.mesh.expired() || !model.mesh.lock())
		return;

	model.lightDepthShader->Use();

	for (std::shared_ptr<Submesh>& smesh : model.mesh.lock()->submeshes)
	{
		if (smesh == nullptr || smesh->gpu.VAO == 0)
			continue;

		Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetWorldMatrix();
		model.lightDepthShader->SetUniform("uModel", modelMat);
		assert(smesh->gpu.VAO != 0);
		i_renderer.RenderMeshOnce(smesh->gpu.VAO, smesh->vertices.size(), 0);
	}
    Shader::Unuse();
}

void StaticDraw::SetMesh(const std::string& i_path)
{
    meshPath = i_path;
    model.SetMeshFromFile(meshPath);
}

void StaticDraw::SetTexture(const std::string& i_path, bool i_flip)
{
    texturePath = i_path;
    model.SetTexture(texturePath, i_flip);
}

void DropOnStaticDrawComponent(unsigned char* io_component, void* io_dropData)
{
    auto& sd = *reinterpret_cast<StaticDraw*>(io_component);
    auto path = *reinterpret_cast<std::filesystem::path*>(io_dropData);
    std::string extension = path.extension().string();
    if (Utils::FindExtension(extension, Utils::MeshExtensions))
    {
        sd.SetMesh(path.string());
    }

    if (Utils::FindExtension(extension, Utils::TextureExtensions))
    {
        sd.SetTexture(path.string(), false);
    }
}

void OnMeshPathUpdate(unsigned char* io_component)
{
    auto& sd = *reinterpret_cast<StaticDraw*>(io_component);
    sd.SetMesh(sd.meshPath);
}

void OnTexturePathUpdate(unsigned char* io_component)
{
    auto& sd = *reinterpret_cast<StaticDraw*>(io_component);
    sd.SetTexture(sd.texturePath, false);
}