#include "vector3.hpp"
#include "vector4.hpp"
#include "utils.hpp"

////////////////////////////// OPERATORS

inline Vector3 Matrix4::operator*(const Vector3& vec) const
{
    const float w = element[3] * vec.x + element[7] * vec.y + element[11] * vec.z + element[15];

    return {
            (element[0] * vec.x + element[4] * vec.y + element[8] * vec.z + element[12]) / w,
            (element[1] * vec.x + element[5] * vec.y + element[9] * vec.z + element[13]) / w,
            (element[2] * vec.x + element[6] * vec.y + element[10] * vec.z + element[14]) / w
    };
}

inline Vector4 Matrix4::operator*(const Vector4& vec) const
{
    Vector4 result = { 0.f, 0.f, 0.f, 0.f };

    for (int column = 0; column < 4; column++)
    {
        for (int line = 0; line < 4; line++)
        {
            result.element[column] += *(&this->line[line].x + column) * vec.element[line];
        }
    }

    return result;
}

inline Matrix4 Matrix4::operator*(const Matrix4& mat) const
{
    Matrix4 result = {};

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            for(int k=0;k<4;k++)
            {
                result.line[i].element[j]+= line[i].element[k] * mat.line[k].element[j];
            }
        }
    }

    return result;
}

inline bool Matrix4::operator==(const Matrix4& mat) const
{
    bool result = true;
    for(int i = 0; i < 16 && result; i++)
        result = Maths::Abs(element[i] - mat.element[i]) <= Maths::Constants::weakEpsilon;

    return result;
}


inline std::ostream& operator<<(std::ostream& o, const Matrix4& mat)
{
    return o << mat.element[0] << ", " << mat.element[1] << ", " << mat.element[2] << ", " << mat.element[3] << ",\n"
             << mat.element[4] << ", " << mat.element[5] << ", " << mat.element[6] << ", " << mat.element[7] << ",\n"
             << mat.element[8] << ", " << mat.element[9] << ", " << mat.element[10] << ", " << mat.element[11] << ",\n"
             << mat.element[12] << ", " << mat.element[13] << ", " << mat.element[14] << ", " << mat.element[15] << "\n";
}

////////////////////////////// FUNCTIONS

inline Matrix4 Matrix4::Identity()
{
    return {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
    };
}

inline Matrix4 Matrix4::Null()
{
    return {
            0.f, 0.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 0.f
    };
}

inline Matrix4 Matrix4::Transpose() const
{
    return {
            element[0], element[4], element[8], element[12],
            element[1], element[5], element[9], element[13],
            element[2], element[6], element[10], element[14],
            element[3], element[7], element[11], element[15]
    };
}

inline Matrix4 Matrix4::Frustum(float i_left, float i_right, float i_bottom, float i_top, float i_near, float i_far)
{
    float rightMinusLeft = i_right - i_left;
    float topMinusBottom = i_top - i_bottom;
    float farMinusNear = i_far - i_near;

    return {
            (2.f * i_near) / rightMinusLeft,      0.f,                                  0.f,                                     0.f,
            0.f,                                  (2.f * i_near) / topMinusBottom,      0.f,                                     0.f,
            (i_right + i_left) / rightMinusLeft,  (i_top + i_bottom) / topMinusBottom,  -(i_far + i_near) / farMinusNear,        -1.f,
            0.f,                                  0.f,                                  -(i_far * i_near * 2.f) / farMinusNear,  0.f
    };
}

inline Matrix4 Matrix4::Orthographic(float i_left, float i_right, float i_bottom, float i_top, float i_near, float i_far)
{
    float rightMinusLeft = i_right - i_left;
    float topMinusBottom = i_top - i_bottom;
    float farMinusNear = i_far - i_near;

    return
            {
                    2.f / rightMinusLeft,                 0.f,                                   0.f,                               0.f,
                    0.f,                                  2.f / topMinusBottom,                  0.f,                               0.f,
                    0.f,                                  0.f,                                   -2 / farMinusNear,                 0.f,
                    -(i_right + i_left) / rightMinusLeft, -(i_top + i_bottom) / topMinusBottom,  -(i_near + i_far) / farMinusNear,  0.f
            };
}

inline Matrix4 Matrix4::Perspective(float fovY, float aspect, float near, float far, bool ortho)
{
    float top = near * Maths::Tan(fovY * 0.5f);

    float right = top * aspect;
    float bottom = -top, left = -right;

    if (ortho)
        return Orthographic(left, right, bottom, top, near, far);
    else
        return Frustum(left, right, bottom, top, near, far);
}

inline Matrix4 Matrix4::RotateX(const float& i_angleRadians)
{
    float cos = Maths::Cos(i_angleRadians), sin = Maths::Sin(i_angleRadians);

    return {
            1.f, 0.f,  0.f, 0.f,
            0.f, cos, -sin, 0.f,
            0.f, sin,  cos, 0.f,
            0.f, 0.f,  0.f, 1.f
    };
}

inline Matrix4 Matrix4::RotateY(const float& i_angleRadians)
{
    float cos = Maths::Cos(i_angleRadians), sin = Maths::Sin(i_angleRadians);

    return {
            cos,  0.f, sin, 0.f,
            0.f,  1.f, 0.f, 0.f,
            -sin, 0.f, cos, 0.f,
            0.f,  0.f, 0.f, 1.f
    };
}

inline Matrix4 Matrix4::RotateZ(const float& i_angleRadians)
{
    float cos = Maths::Cos(i_angleRadians), sin = Maths::Sin(i_angleRadians);

    return {
            cos, -sin, 0.f, 0.f,
            sin, cos,  0.f, 0.f,
            0.f, 0.f,  1.f, 0.f,
            0.f, 0.f,  0.f, 1.f
    };
}

inline Matrix4 Matrix4::RotateXYZ(float i_xRadAngle, float i_yRadAngle, float i_zRadAngle)
{
    return  RotateY(i_yRadAngle) * RotateX(i_xRadAngle) * RotateZ(i_zRadAngle);
}

inline Matrix4 Matrix4::RotateXYZ(const Vector3& i_anglesRadians)
{
    return  RotateY(i_anglesRadians.y) * RotateX(i_anglesRadians.x) * RotateZ(i_anglesRadians.z);
}

inline Matrix4 Matrix4::Translate(float i_x, float i_y, float i_z)
{
    return {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            i_x, i_y, i_z, 1.f
    };
}

inline Matrix4 Matrix4::Translate(const Vector3& i_vec)
{
    return {
            1.f,     0.f,     0.f,     0.f,
            0.f,     1.f,     0.f,     0.f,
            0.f,     0.f,     1.f,     0.f,
            i_vec.x, i_vec.y, i_vec.z, 1.f
    };
}

inline Matrix4 Matrix4::Scale(const float& i_sc)
{
    return {
            i_sc, 0.f,  0.f,  0.f,
            0.f,  i_sc, 0.f,  0.f,
            0.f,  0.f,  i_sc, 0.f,
            0.f,  0.f,  0.f,  1.f
    };
}

inline Matrix4 Matrix4::Scale(const Vector3& i_sc)
{
    return {
            i_sc.x,  0.f,     0.f,     0.f,
            0.f,     i_sc.y,  0.f,     0.f,
            0.f,     0.f,     i_sc.z,  0.f,
            0.f,     0.f,     0.f,     1.f
    };
}