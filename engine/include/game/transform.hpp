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

        PROPERTY(Vector3, position);
        GET(position){
            return _position;
        }
        SET(position){
            if (_position != value)
                needUpdate = true;
            _position = value;
        }

        PROPERTY(Vector3, rotation);
        GET(rotation)
        {
            return _rotation;
        }
        SET(rotation)
        {
            if (_rotation != value)
                needUpdate = true;
            _rotation = value;
        }

        PROPERTY(Vector3, scale);
        GET(scale){
            return _scale;
        }
        SET(scale)
        {
            if (_scale != value)
                needUpdate = true;
            _scale = value;
            scaleLockParams.origScale = _scale;
            scaleLockParams.ratio = 1.f;
        }

        WRITEONLY_PROPERTY(Transform*, parent);
        SET(parent)
        {
            _parent = value;
            if (value)
                value->childs.emplace_back(this);
        }

		/**
		 * @brief Get a model matrix from a transform. If model matrix not update, compute the matrix.
		 * @return Matrix4 model
		 */
		[[nodiscard]] Matrix4 GetModelMatrix() const;

        void RemoveChild(Transform* childToRemove);

        /**
         * @return the ScaleLockParameters struct of this transform
         */
        [[nodiscard]] ScaleLockParams& GetScaleLockParameters() { return scaleLockParams; }

    private:
        mutable bool needUpdate = true;

		Vector3 _position = Vector3::zero;
		Vector3 _rotation = Vector3::zero;
		Vector3 _scale = { 1.f, 1.f, 1.f };

        ScaleLockParams scaleLockParams;

        mutable Matrix4 modelMatrix = Matrix4::Identity();

        Transform* _parent = nullptr;
        std::list<Transform*> childs;

        /**
         * Compute the model matrix using position, rotation and scale
         */
        void UpdateModelMatrix() const;
	};
};