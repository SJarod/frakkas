#include "renderer/lowlevel/camera.hpp"

Matrix4 Renderer::LowLevel::Camera::GetViewMatrix() const
{
	Vector3 rotation = { Maths::ToRadians(transform.rotation.x),
						 Maths::ToRadians(-transform.rotation.y),
						 Maths::ToRadians(transform.rotation.z) };
	return Matrix4::Translate(-transform.position) * Matrix4::RotateXYZ(rotation);
}

Matrix4 Renderer::LowLevel::Camera::GetProjectionMatrix(float i_aspectRatio) const
{
	return Matrix4::Perspective(fovY, i_aspectRatio, near, far, false);
}

void Renderer::LowLevel::Camera::InspectorUpdate()
{
	//ImGui::Begin("Camera");
	//ImGui::DragFloat3("camera position", transform.position.element, 1.f);
	//ImGui::DragFloat3("camera rotation", transform.rotation.element, 0.1f, -180.f, 180.f);
	//ImGui::SliderAngle("fov", &fovY, 0.f, 180.f);
	//ImGui::DragFloat("near", &near, 0.01f);
	//ImGui::DragFloat("far", &far, 0.01f);
	//ImGui::End();
}