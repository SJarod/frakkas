#include <imgui.h>

#include "resources/serializer.hpp"

#include "game/transform.hpp"

using namespace Game;

Matrix4 Transform::GetModelMatrix() const
{
    bool parentUpdate = (_parent && _parent->needUpdate);
    if (needUpdate || parentUpdate)
        UpdateModelMatrix();
    return modelMatrix;
}

void Transform::RemoveChild(Transform* childToRemove)
{
    std::list<Transform*>::iterator it;
    it = std::find(childs.begin(), childs.end(), childToRemove);

    if (it != childs.end())
        childs.erase(it);
}

void Transform::UpdateModelMatrix() const {
    modelMatrix =  Matrix4::Scale(scale) * Matrix4::RotateXYZ(rotation) * Matrix4::Translate(position);
    if (_parent)
        modelMatrix = modelMatrix * _parent->GetModelMatrix();

    needUpdate = false;
    for (Transform* child: childs)
        child->needUpdate = true;
}

void Game::Transform::Edit() {
    ImGui::Text("Transform");

    ImGui::Spacing();

    Vector3 pos = position, rot = Maths::ToDegrees(rotation), sc = scale;
    // position edit
    ImGui::DragFloat3("Position", pos.element, 0.1f);
    // rotation edit
    ImGui::DragFloat3("Rotation", rot.element, 1.f);
    for (float & coord : rot.element)
        coord = Maths::Abs(coord) >= 360.f ? 0.f : coord;
    // scale edit
    ScaleEdit(sc);

    position = pos;
    rotation = Maths::ToRadians(rot);
    scale = sc;
}

void Transform::ScaleEdit(Vector3 &sc) {
    if(scalePropLock && scaleLockOrig == Vector3::zero)
        scaleRatio = 0.f;

    if(ImGui::DragFloat3("Scale", sc.element, 0.1f))
    {
        if (scalePropLock)
        {
            if (sc.x != _scale.x && scaleLockOrig.x != 0.f)
                scaleRatio = sc.x / scaleLockOrig.x;
            else if (sc.y != _scale.y && scaleLockOrig.y != 0.f)
                scaleRatio = sc.y / scaleLockOrig.y;
            else if (scaleLockOrig.z != 0.f)
                scaleRatio = sc.z / scaleLockOrig.z;
            else
                scaleRatio = sc.x + sc.y + sc.z;
        }
    }

    if (scalePropLock && scaleLockOrig != Vector3::zero)
        sc = scaleLockOrig * scaleRatio;
    else if (scalePropLock && scaleLockOrig == Vector3::zero)
        sc = Vector3(scaleRatio, scaleRatio, scaleRatio);

    ImGui::SameLine();
    ImGui::Checkbox("Lock scale", &scalePropLock);
}
