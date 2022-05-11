#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "resources/resource.hpp"
#include "resources/skeletal_mesh.hpp"
#include "maths.hpp"

namespace Resources
{
    struct KeyFramePos
    {
        Vector3 pos;
        float timeStamp;
    };

    struct KeyFrameRot
    {
        Quaternion rot;
        float timeStamp;
    };

    struct KeyFrameScl
    {
        Vector3 scl;
        float timeStamp;
    };

    class KeyFrameBone
    {
    private:
        std::vector<KeyFramePos> positions;
        std::vector<KeyFrameRot> rotations;
        std::vector<KeyFrameScl> scales;

        std::string_view name;
        int id;

        /**
         * Get a scale factor depending on two time stamps and a general time.
         */
        float GetLerpFactor(const float i_lastTimeStamp, const float i_nextTimeStamp, const float i_animationTime) const;

        /**
         * Get the translation matrix of a bone with its value interpolated between two time stamps, depending on the animation time.
         */
        Matrix4 InterpolatePosition(const float i_animationTime) const;

        /**
         * Get the rotation matrix of a bone with its value interpolated between two time stamps, depending on the animation time.
         */
        Matrix4 InterpolateRotation(const float i_animationTime) const;

        /**
         * Get the scale matrix of a bone with its value interpolated between two time stamps, depending on the animation time.
         */
        Matrix4 InterpolateScale(const float i_animationTime) const;

    public:
        /**
         * Create a bone containing every key frame values with an Assimp channel.
         * Assimp channels are bones.
         */
        KeyFrameBone(const std::string_view& i_name, const int i_id, const aiNodeAnim* i_assimpChannel);

        /**
         * Store every interpolated transforms in output matrices.
         */
        void InterpolateTransform(const float i_animationTime, Matrix4& o_t, Matrix4& o_r, Matrix4& o_s) const;

        /**
         * Get the key position's index depending on the animation time.
         */
        int GetPositionKeyFrame(const float i_animationTime) const;

        /**
         * Get the key rotation's index depending on the animation time.
         */
        int GetRotationKeyFrame(const float i_animationTime) const;

        /**
         * Get the key scale's index ddepending on the animation time.
         */
        int GetScaleKeyFrame(const float i_animationTime) const;

        /**
         * Get this bone name.
         */
        const std::string_view& GetName() const;
    };

    /**
     * Structure used to store the skeleton's hierarchy.
     */
    struct SkeletonNodeData
    {
        Matrix4 transform;
        std::string_view name;

        int childrenCount;
        std::vector<SkeletonNodeData> children;
    };

    /**
     * One animation found in the animation file.
     */
    class SkeletalAnimation
    {
    private:
        std::vector<KeyFrameBone> kfBones;
        SkeletonNodeData rootNode;
        std::unordered_map<std::string_view, Bone> boneInfoMap;

        const int animationIndex = 0;

        /**
         * Load the skeleton node data with Assimp's data.
         */
        void ReadHierarchyData(SkeletonNodeData& o_dest, const aiNode* i_src);

        /**
         * Repair the skeletal mesh bone info if bones are missing.
         */
        void ReadMissingBones(const aiAnimation* i_animation, SkeletalMesh& io_skmesh);

    public:
        const std::string animationName;

        float duration = 0.f;
        //framerate
        float tick = 0.f;

        SkeletalAnimation(const std::string& i_name, const aiScene* i_scene, SkeletalMesh& i_skmesh, const int i_animationIndex);

        /**
         * Look for a bone in the bone map with its name.
         */
        const KeyFrameBone* FindBone(const std::string_view& i_name) const;

        /**
         * Get a reference to the entire bone map.
         */
        const std::unordered_map<std::string_view, Bone>& GetBoneMap() const;

        /**
         * Get the root node of the skeleton's data.
         */
        const SkeletonNodeData* GetRootNode() const;
    };

    /**
     * A skeletal animation pack is a set of animations, representing one animation file.
     * There can be multiple animations in one file.
     * It is made from and for a specific skeletal mesh, it is stored as a resource.
     */
    class SkeletalAnimationPack : public Resources::Resource
    {
    private:
#ifdef ANIMATION_MAP
        std::unordered_map<std::string_view, SkeletalAnimation> animations;
#else
        std::vector<SkeletalAnimation> animations;
#endif

        SkeletalMesh& mappedSkmesh;

    public:
        const std::string animationFilename;

        SkeletalAnimationPack(const std::string& i_name, const std::string& i_animationFilename, SkeletalMesh& i_skmesh);

        void LoadFromInfo() override;

#ifdef ANIMATION_MAP
        /**
         * Get an animation specifying its name.
         */
        const SkeletalAnimation* GetAnimation(const std::string_view& i_animationName) const;
#else
        /**
         * Get an animation specifying its index.
         */
        const SkeletalAnimation* GetAnimation(const unsigned int i_index) const;
#endif

        const int GetPackSize() const;
    };
}