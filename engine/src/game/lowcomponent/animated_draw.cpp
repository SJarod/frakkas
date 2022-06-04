#include <cassert>
#include <filesystem>

#include "game/entity.hpp"
#include "game/time_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/light.hpp"
#include "resources/resources_manager.hpp"
#include "resources/skeletal_mesh.hpp"

#include "utils/dragdrop_constants.hpp"
#include "renderer/model.hpp"
#include "game/lowcomponent/animated_draw.hpp"

KK_COMPONENT_IMPL_BEGIN(AnimatedDraw)
KK_FIELD_PUSH(AnimatedDraw, meshPath, EDataType::STRING)
KK_FIELD_CHANGECALLBACK(OnAnimationMeshPathUpdate)
KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnAnimatedDrawComponentMesh)

KK_FIELD_PUSH(AnimatedDraw, texturePath, EDataType::STRING)
KK_FIELD_CHANGECALLBACK(OnAnimationTexturePathUpdate)
KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnAnimatedDrawComponentTexture)

KK_FIELD_PUSH(AnimatedDraw, skmodel, EDataType::SKELETON)
KK_FIELD_PUSH(AnimatedDraw, animGraph, EDataType::ANIMATION)
KK_COMPONENT_IMPL_END

void AnimatedDraw::OnUpdate()
{
	animGraph.player.UpdatePlayer(Game::Time::GetDeltaTime());
}

void AnimatedDraw::Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform)
{
	if (skmodel.skmesh.expired() || !skmodel.skmesh.lock())
		return;

	skmodel.UseShader();
	skmodel.SetUniform("uBoneTransforms", MAX_BONES, animGraph.player.GetGlobalBoneMatrices().data()->element);

	{
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
	}

	for (Renderer::Socket& socket : skmodel.sockets)
	{
		GLuint texToBeBinded = ResourcesManager::GetDefaultTexture().data;

		Resources::Texture* diffuseTex = socket.model.diffuseTex.lock().get();
		if (diffuseTex != nullptr)
			if (diffuseTex->gpu.get())
				texToBeBinded = diffuseTex->gpu->data;

		socket.model.UseShader();

		Matrix4 socketTRS = Matrix4::Scale(socket.transform.scale) *
			Matrix4::RotateXYZ(socket.transform.rotation) *
			Matrix4::Translate(socket.transform.position);
		Matrix4 boneTRS = animGraph.player.GetRawBoneMatrices()[socket.boneID];

		for (std::shared_ptr<Submesh>& smesh : socket.model.mesh.lock()->submeshes)
		{
			if (smesh == nullptr || smesh->gpu.VAO == 0)
				continue;

			Matrix4 modelMat = socketTRS * boneTRS * smesh->localTransform * i_entityTransform.GetWorldMatrix();
			socket.model.SetUniform("uModel", modelMat);
			Matrix4 modelNormal = (modelMat.Inverse()).Transpose();
			socket.model.SetUniform("uModelNormal", modelNormal);

			assert(smesh->gpu.VAO != 0);
			i_renderer.RenderMeshOnce(smesh->gpu.VAO,
				smesh->vertices.size(),
				texToBeBinded);

			if (i_renderer.outline)
				i_renderer.RenderMeshOnceOutline(smesh->gpu.VAO, smesh->vertices.size());
		}
	}

	Resources::Shader::Unuse();
}

void AnimatedDraw::DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform)
{
	if (skmodel.skmesh.expired() || !skmodel.skmesh.lock())
		return;

	skmodel.lightDepthShader->Use();
	skmodel.lightDepthShader->SetUniform("uBoneTransforms",
		MAX_BONES,
		animGraph.player.GetGlobalBoneMatrices().data()->element);

	for (std::shared_ptr<Submesh>& smesh : skmodel.skmesh.lock()->submeshes)
	{
		if (smesh == nullptr || smesh->gpu.VAO == 0)
			continue;

		Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetWorldMatrix();
		skmodel.lightDepthShader->SetUniform("uModel", modelMat);
		assert(smesh->gpu.VAO != 0);
		i_renderer.RenderMeshOnce(smesh->gpu.VAO, smesh->vertices.size(), 0);
	}

	for (Renderer::Socket& socket : skmodel.sockets)
	{
		socket.model.lightDepthShader->Use();

		Matrix4 socketTRS = Matrix4::Scale(socket.transform.scale) *
			Matrix4::RotateXYZ(socket.transform.rotation) *
			Matrix4::Translate(socket.transform.position);
		Matrix4 boneTRS = animGraph.player.GetRawBoneMatrices()[socket.boneID];

		for (std::shared_ptr<Submesh>& smesh : socket.model.mesh.lock()->submeshes)
		{
			if (smesh == nullptr || smesh->gpu.VAO == 0)
				continue;

			Matrix4 modelMat = socketTRS * boneTRS * smesh->localTransform * i_entityTransform.GetWorldMatrix();
			socket.model.lightDepthShader->SetUniform("uModel", modelMat);
			assert(smesh->gpu.VAO != 0);
			i_renderer.RenderMeshOnce(smesh->gpu.VAO, smesh->vertices.size(), 0);
		}
	}

	Shader::Unuse();
}

void AnimatedDraw::SetMesh(const std::string& i_path)
{
	meshPath = i_path;
	skmodel.SetSkeletalMeshFromFile(i_path);
	animGraph.SetOwningSkeletalMesh(skmodel.skmesh.lock());
}

void AnimatedDraw::SetTexture(const std::string& i_path, bool i_flip)
{
	texturePath = i_path;
	skmodel.SetTexture(i_path, i_flip);
}

void AnimatedDraw::SetAnimation(const std::string& i_path)
{
	animGraph.AddAnimationPack(i_path);
}

void DropOnAnimatedDrawComponentMesh(unsigned char* io_component, void* io_dropData)
{
	auto& ad = *reinterpret_cast<AnimatedDraw*>(io_component);
	auto path = *reinterpret_cast<std::filesystem::path*>(io_dropData);
	std::string extension = path.extension().string();
	if (Utils::FindExtension(extension, Utils::MeshExtensions))
	{
		ad.SetMesh(path.string());
	}
}

void DropOnAnimatedDrawComponentTexture(unsigned char* io_component, void* io_dropData)
{
	auto& ad = *reinterpret_cast<AnimatedDraw*>(io_component);
	auto path = *reinterpret_cast<std::filesystem::path*>(io_dropData);
	std::string extension = path.extension().string();
	if (Utils::FindExtension(extension, Utils::TextureExtensions))
	{
		ad.SetTexture(path.string(), false);
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