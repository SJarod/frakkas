#pragma once

#include "maths.hpp"
#include "physics/transform.hpp"

namespace Renderer
{
	namespace LowLevel
	{
		class Camera
		{
		public:
			/**
			 * Get camera's view matrix.
			 * 
			 */
			Matrix4 GetViewMatrix() const;

			/**
			 * Get camera's projection matrix.
			 * 
			 * @param i_aspectRatio
			 */
			Matrix4 GetProjectionMatrix(float i_aspectRatio) const;

			/**
			 * Create an ImGui window to change camera's values.
			 * 
			 */
			void InspectorUpdate();

			Physics::Transform transform;
		private:

			float fovY = Maths::Constants::piHalf;
			float near = 0.001f;
			float far = 1000.f;
		};
	};
};