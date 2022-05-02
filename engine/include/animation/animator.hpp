#pragma once

namespace Resources
{
	struct Bone;
	class SkeletalMesh;

	struct SkeletonNodeData;
	class SkeletalAnimation;
	class SkeletalAnimationPack;
}

using namespace Resources;

namespace Animation
{
	/**
	 * Play a skeletal animation.
	 */
	class Animator
	{
	private:
		float playSpeed = 1.f;
		float playTime = 0.f;

		// Vector of every bones, stored as transform matrix
		std::vector<Matrix4> boneMatrices;

		/**
		 * Process every bone's matrix to make them global.
		 * Update the values in the boneMatrices vector.
		 * This function is recursive.
		 * 
		 * @param SkeletonNodeData*
		 * @param i_parentTransform
		 */
		void UpdateBoneMatrices(const SkeletonNodeData* i_node, const Matrix4& i_parentTransform = Matrix4::Identity());

	public:
		const SkeletalAnimation* anim = nullptr;
		const std::unordered_map<std::string_view, Resources::Bone>* animBoneInfo = nullptr;

		/**
		 * Create the animator, allocating memory for MAX_BONES matrices.
		 */
		Animator();

		/**
		 * Pass a SkeletalAnimation to this animator and reset the play time.
		 * 
		 * @param SkeletalAnimation*
		 */
		void UploadAnimation(const SkeletalAnimation* i_anim);

		/**
		 * Animator's update, called in the Animated component.
		 * 
		 * @param i_deltaTime
		 */
		void UpdatePlayer(const float& i_deltaTime);

		/**
		 * Get the vector of bone transform matrices.
		 */
		const std::vector<Matrix4>& GetBoneMatrices() const;
	};
}