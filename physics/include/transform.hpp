#pragma once

#include "maths.hpp"

namespace Physics
{
	struct Transform
	{
	public:
		Vector3 position = Vector3::zero;
		Vector3 rotation = Vector3::zero;
		Vector3 scale = { 1.f, 1.f, 1.f };

		/**
		 * Get a model matrix from a transform.
		 * 
		 * @return Matrix4
		 */
		inline Matrix4 GetModelMatrix() const
		{
			return Matrix4::Scale(scale) * Matrix4::RotateXYZ(rotation) * Matrix4::Translate(position);
		}
	};
};