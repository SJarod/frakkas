#include "renderer/lowlevel/camera.hpp"


Matrix4 Renderer::LowLevel::Camera::GetViewMatrix() const
{
    Vector3 pos = transform.position, rot = transform.rotation;
    if (transform.parent != nullptr)
    {
        pos = pos + transform.parent.get()->position.get();
        rot = rot + transform.parent.get()->rotation.get();
    }
	Vector3 rotation = { rot.x,
						 -rot.y,
						 rot.z };
	return Matrix4::Translate(-pos) * Matrix4::RotateXYZ(rot);
}

Matrix4 Renderer::LowLevel::Camera::GetProjectionMatrix(float i_aspectRatio) const
{
	return Matrix4::Perspective(fovY, i_aspectRatio, near, far, false);
}

void Renderer::LowLevel::Camera::SetFieldOfView(const float i_fovY)
{
	fovY = i_fovY;
}
