#pragma once

#include <list>

#include "utils/properties.hpp"
#include "maths.hpp"


namespace Resources
{
    class Serializer;
}

namespace Game
{
    /**
     * @brief ScaleLockParams is a struct containing parameters about transform scale when scale is locked.
     * In this case, the scale vector must keep the same direction.
     * It is used only for editing, so this structure is a bit odd but necessary.
     * @param isLock indicate if scale is locked
     * @param ratio the ratio between the current coordinate value and the origin coordinate value
     * @param origScale the origin scale coordinates values. Useful for ratio computing
     */
    struct ScaleLockParams
    {
        bool isLock = true;
        float ratio = 1.f;
        Vector3 origScale = {1.f, 1.f, 1.f };
    };

	struct Transform
	{
	public:
        Transform();


        Property<Vector3> position;
        Property<Vector3> rotation;
        Property<Vector3> scale;

        Property<Transform*> parent;

		/**
		 * @brief Get a model matrix from a transform. If model matrix not update, compute the matrix.
		 * @return Matrix4 model
		 */
		Matrix4 GetModelMatrix() const;

        /**
         * @brief Get a model matrix with uniform scale from a transform. If model matrix not update, compute the matrix.
         * If the transform's scale is not uniform, we will scale uniformly according to greatest scale.
         * @return Matrix4 model
         */
        Matrix4 GetModelMatrixUniformScale() const;

        void RemoveChild(Transform* childToRemove);
        /**
         * Remove all child. Put their parent to nullptr.
         */
        void ClearChilds();

        /**
         * @return the ScaleLockParameters struct of this transform
         */
        ScaleLockParams& GetScaleLockParameters() { return scaleLockParams; }

    private:
        mutable bool needUpdate = true;
        mutable bool needUniformUpdate = true;

        ScaleLockParams scaleLockParams;

        mutable Matrix4 modelMatrix = Matrix4::Identity();

        std::list<Transform*> childs;

        /**
         * Compute the model matrix using position, rotation and scale
         */
        void UpdateModelMatrix() const;

        /**
         * Compute the model matrix using position, rotation and uniform scale.
         * If the transform's scale is not uniform, we will scale uniformly according to greatest scale.
         */
        void UpdateModelMatrixUniformScale() const;
	};
}
