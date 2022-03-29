#pragma once

#include <list>

#include "properties.hpp"
#include "maths.hpp"

namespace Game
{
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
        GET(rotation){
            return _rotation;
        }
        SET(rotation){
            if (_rotation != value)
                needUpdate = true;
            _rotation = value;
        }

        PROPERTY(Vector3, scale);
        GET(scale){
            return _scale;
        }
        SET(scale){
            if (_scale != value)
                needUpdate = true;
            _scale = value;
        }

        WRITEONLY_PROPERTY(Transform*, parent);
        SET(parent){
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

    private:
        mutable bool needUpdate = true;

		Vector3 _position = Vector3::zero;
		Vector3 _rotation = Vector3::zero;
		Vector3 _scale = { 1.f, 1.f, 1.f };

        mutable Matrix4 modelMatrix = Matrix4::Identity();

        Transform* _parent = nullptr;
        std::list<Transform*> childs;

        /**
         * Compute the model matrix using position, rotation and scale
         */
        void UpdateModelMatrix() const;
	};
};
