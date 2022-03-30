#include "imgui.h"

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
