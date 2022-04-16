#include <imgui.h>

#include "game/entity.hpp"

#include "game/camera_component.hpp"

KK_COMPONENT_IMPL(CameraComponent)
KK_FIELD_IMPL(CameraComponent, camera, DataType::CAMERA, 1)

void CameraComponent::SetOwner(Entity* owner)
{
    camera.transform.parent = &owner->transform;
    Component::SetOwner(owner);
}
