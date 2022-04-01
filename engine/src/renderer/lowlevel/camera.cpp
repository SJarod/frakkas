#include "renderer/lowlevel/camera.hpp"


Matrix4 Renderer::LowLevel::Camera::GetViewMatrix() const
{
	Vector3 rotation = { transform.rotation.x, -transform.rotation.y, transform.rotation.z };

	return Matrix4::Translate(-transform.position) * Matrix4::RotateXYZ(rotation);
}

Matrix4 Renderer::LowLevel::Camera::GetProjectionMatrix(float i_aspectRatio) const
{
	return Matrix4::Perspective(fovY, i_aspectRatio, near, far, false);
}

void Renderer::LowLevel::Camera::SetFieldOfView(const float i_fovY)
{
	fovY = i_fovY;
}