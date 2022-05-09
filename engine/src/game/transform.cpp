#include "game/transform.hpp"

using namespace Game;

Transform::Transform()
{
    position = Vector3::zero;
    rotation = Vector3::zero;
    scale = Vector3(1.f, 1.f, 1.f);
    position.setter = [&](const Vector3& value)
    {
        if (position != value)
            needUpdate = needUniformUpdate = true;
        position.set(value);
    };
    rotation.setter = [&](const Vector3& value)
    {
        if (rotation != value)
            needUpdate = needUniformUpdate = true;
        rotation.set(value);
    };
    scale.setter = [&](const Vector3& value)
    {
        if (scale != value)
            needUpdate = needUniformUpdate = true;
        scale.set(value);
        scaleLockParams.origScale = scale.get();
        scaleLockParams.ratio = 1.f;
    };
    parent.setter = [&](Transform* value)
    {
        // Check if this transform, is not a parent of the input transform 'value'
        if (value)
        {
            bool isParent = false;
            Transform* parent = value->parent;
            while (parent)
            {
                if (this == parent) isParent = true;
                parent = parent->parent;
            }
            if (isParent) return;
        }

        parent.set(value);
        needUpdate = needUniformUpdate = true;
        if (value && value)
            value->childs.emplace_back(this);
    };
    parent = nullptr;
}

Matrix4 Transform::GetModelMatrix() const
{
    bool parentUpdate = (parent.get() && parent.get()->needUpdate);

    if (needUpdate || parentUpdate)
        UpdateModelMatrix();

    return modelMatrix;
}

Matrix4 Transform::GetModelMatrixUniformScale() const
{
    bool parentUpdate = (parent.get() && parent.get()->needUniformUpdate);

    if (needUniformUpdate || parentUpdate)
        UpdateModelMatrixUniformScale();

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
    if (parent)
        modelMatrix = modelMatrix * parent.get()->GetModelMatrix();

    needUpdate = false;
    for (Transform* child: childs)
        child->needUpdate = true;
}

void Transform::UpdateModelMatrixUniformScale() const
{
    float maxValue = Maths::Max(scale.get().x, scale.get().y, scale.get().z);
    Vector3 uniformScale{ maxValue,maxValue ,maxValue };

    modelMatrix = Matrix4::Scale(uniformScale) * Matrix4::RotateXYZ(rotation) * Matrix4::Translate(position);
    if (parent)
        modelMatrix = modelMatrix * parent.get()->GetModelMatrixUniformScale();

    needUniformUpdate = false;
    for (Transform* child : childs)
        child->needUniformUpdate = true;
}

void Transform::ClearChilds()
{
    for (Transform* child : childs)
        child->parent = nullptr;

    childs.clear();
}
