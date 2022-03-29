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
			 * Get camera's view matrix.
			 * 
			 */
			[[nodiscard]] Matrix4 GetViewMatrix() const;

			/**
			 * Get camera's projection matrix.
			 * 
			 * @param i_aspectRatio
			 */
			[[nodiscard]] Matrix4 GetProjectionMatrix(float i_aspectRatio) const;

			/**
			 * Change the camera's field of view.
			 * 
			 * @param i_fovY vertical field of view
			 */
			void SetFieldOfView(const float i_fovY);

			Game::Transform transform;

			//target vertical field of view
			float targetFovY = Maths::Constants::piHalf;

		private:
			//vertical field of view
			float fovY = 0.f;

			float near = 0.001f;
			float far = 1000.f;
		};
	};
};
