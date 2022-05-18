#include "imgui.h"

#include "game/entity.hpp"
#include "game/lowcomponent/camera.hpp"

KK_COMPONENT_IMPL(Camera)
KK_FIELD_RANGE_IMPL(Camera, near, EDataType::FLOAT, 1, 0.001f, 10.f)
KK_FIELD_RANGE_IMPL(Camera, far, EDataType::FLOAT, 1, 100.f, 5000.f)
KK_FIELD_RANGE_IMPL(Camera, fovY, EDataType::FLOAT, 1, 0.f, 180.f)


Matrix4 Camera::GetViewMatrix() const
{
    Vector3 pos = owner.get()->transform.position, rot = owner.get()->transform.rotation;

    Vector3 invertRot = { rot.x, -rot.y, rot.z };
    return Matrix4::Translate(-pos) * Matrix4::RotateXYZ(invertRot);
}

Matrix4 Camera::GetProjectionMatrix(float i_aspectRatio) const
{
    return Matrix4::Perspective(Maths::ToRadians(fovY), i_aspectRatio, near, far, false);
}

Matrix4 Camera::GetCascadeProjectionMatrix(float i_aspectRatio, float i_near, float i_far) const
{
    return Matrix4::Perspective(Maths::ToRadians(fovY), i_aspectRatio, i_near, i_far, false);
}