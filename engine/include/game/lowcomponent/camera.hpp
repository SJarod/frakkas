#pragma once

#include "maths.hpp"

#include "game/component_generator.hpp"

namespace Game
{
    KK_COMPONENT(Camera)

		KK_FIELD(float, near) = 0.001f;
		KK_FIELD(float, far) = 1000.f;

		//vertical field of view in degrees, you should convert in radians for calcul
		KK_FIELD(float, fovY) = 90.f;

		/**
		* @Summary Get camera's view matrix.
		*/
		Matrix4 GetViewMatrix() const;

		/**
		 * @Summary Get camera's projection matrix.
		 * @param i_aspectRatio
		 */
		Matrix4 GetProjectionMatrix(float i_aspectRatio) const;

    KK_COMPONENT_END
}


