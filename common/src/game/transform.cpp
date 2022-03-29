//
// Created by flori on 3/27/2022.
//

#include "game/transform.hpp"

using namespace Game;

Matrix4 Transform::GetModelMatrix() const
{
    if (needUpdate)
        UpdateModelMatrix();
    return modelMatrix;
}

void Transform::RemoveChild(Transform* childToRemove)
{
    childs.remove(childToRemove);
}

void Transform::UpdateModelMatrix() const {
    modelMatrix =  Matrix4::Scale(scale) * Matrix4::RotateXYZ(rotation) * Matrix4::Translate(position);
    if (_parent)
        modelMatrix = modelMatrix * _parent->modelMatrix;

    needUpdate = false;
    for (Transform* child: childs)
        child->needUpdate = true;
}
