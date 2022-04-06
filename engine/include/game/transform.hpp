#pragma once

#include <list>

#include "properties.hpp"
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

        void RemoveChild(Transform* childToRemove);

        /**
         * @return the ScaleLockParameters struct of this transform
         */
        ScaleLockParams& GetScaleLockParameters() { return scaleLockParams; }

    private:
        mutable bool needUpdate = true;

        ScaleLockParams scaleLockParams;

        mutable Matrix4 modelMatrix = Matrix4::Identity();

        std::list<Transform*> childs;

        /**
         * Compute the model matrix using position, rotation and scale
         */
        void UpdateModelMatrix() const;
	};
};
