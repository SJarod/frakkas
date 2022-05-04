#include "imgui.h"

#include "game/entity.hpp"

#include "game/lowcomponent/camera_component.hpp"

KK_COMPONENT_IMPL(CameraComponent)
KK_FIELD_IMPL(CameraComponent, camera, EDataType::CAMERA, 1)

void CameraComponent::SetOwner(Entity* owner)
{
    camera.transform.parent = &owner->transform;
    Component::SetOwner(owner);
}
