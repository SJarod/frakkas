#include "game/transform.hpp"


using namespace Game;

Transform::Transform() {

    scale = Vector3(1.f, 1.f, 1.f);
    position.setter = [&](const Vector3& value)
    {
        if (position != value)
            needUpdate = true;
        position.set(value);
    };
    rotation.setter = [&](const Vector3& value)
    {
        if (rotation != value)
            needUpdate = true;
        rotation.set(value);
    };
    scale.setter = [&](const Vector3& value)
    {
        if (scale != value)
            needUpdate = true;
        scale.set(value);
        scaleLockParams.origScale = scale.get();
        scaleLockParams.ratio = 1.f;
    };
    parent.setter = [&](Transform* value)
    {
        parent.set(value);
        if (value)
            value->childs.emplace_back(parent.get());
    };
}

Matrix4 Transform::GetModelMatrix() const
{
    bool parentUpdate = (parent.get() && parent.get()->needUpdate);

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

void Transform::UpdateModelMatrix() const
{
    modelMatrix =  Matrix4::Scale(scale) * Matrix4::RotateXYZ(rotation) * Matrix4::Translate(position);
    if (parent.get())
        modelMatrix = modelMatrix * parent.get()->GetModelMatrix();

    needUpdate = false;
    for (Transform* child: childs)
        child->needUpdate = true;
}
