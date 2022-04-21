#pragma once

#include "maths.hpp"
#include "game/transform.hpp"


namespace Renderer
{
	namespace LowLevel
	{
		class Camera
		{
		public:
			/**
			 * @Summary Get camera's view matrix.
			 */
			Matrix4 GetViewMatrix() const;

			/**
			 * @Summary Get camera's projection matrix.
			 * @param i_aspectRatio
			 */
			Matrix4 GetProjectionMatrix(float i_aspectRatio) const;

			/**
			 * @Summary Change the camera's field of view.
			 * @param i_fovY vertical field of view
			 */
			void SetFieldOfView(const float i_fovY);

			Game::Transform transform;

			float targetFovY = Maths::Constants::piHalf; //target vertical field of view
			float near = 0.001f;
			float far = 1000.f;

			//vertical field of view
			float fovY = Maths::Constants::piHalf;
		private:

		};
	}
}
