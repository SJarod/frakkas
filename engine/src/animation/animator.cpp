#include "debug/log.hpp"

#include "resources/skeletal_mesh.hpp"
#include "resources/skeletal_animation.hpp"

#include "animation/animator.hpp"

Animation::Animator::Animator()
{
	boneMatrices.reserve(MAX_BONES);
	for (unsigned int i = 0; i < MAX_BONES; ++i)
	{
		boneMatrices.push_back(Matrix4::Identity());
	}
}

void Animation::Animator::UploadAnimation(const SkeletalAnimation* anim)
{
	playTime = 0.f;
	this->anim = anim;
	animBoneInfo = &anim->GetBoneMap();
}

void Animation::Animator::UpdatePlayer(const float& deltaTime)
{
	if (!anim)
		return;

	float rewind = playSpeed >= 0.f ? 0.f : 1.f;
	float rewindOffset = rewind * anim->duration;

	playTime += deltaTime * anim->tick * playSpeed + rewindOffset;
	playTime = std::fmodf(playTime, anim->duration);

	UpdateBoneMatrices(anim->GetRootNode());
}

const std::vector<Matrix4>& Animation::Animator::GetBoneMatrices() const
{
	return boneMatrices;
}

void Animation::Animator::UpdateBoneMatrices(const SkeletonNodeData* node, const Matrix4& parentTransform)
{
	std::string_view nodeName = node->name;
	Matrix4 nodeTransform = node->transform.Transpose();

	const KeyFrameBone* kfBone = anim->FindBone(nodeName);

	if (kfBone)
	{
		Matrix4 t, r, s;
		kfBone->InterpolateTransform(playTime, t, r, s);
		nodeTransform = s * r * t;
	}

	Matrix4 globalTransformation = nodeTransform * parentTransform;

	if (animBoneInfo->find(nodeName) != animBoneInfo->end())
	{
		int index = animBoneInfo->find(nodeName)->second.id;
		Matrix4 offset = animBoneInfo->find(nodeName)->second.offset.Transpose();
		boneMatrices[index] = offset * globalTransformation;
	}

	for (unsigned int i = 0; i < node->childrenCount; ++i)
		UpdateBoneMatrices(&node->children[i], globalTransformation);
}