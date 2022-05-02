#include "maths/vector3.hpp"
#include "maths/vector4.hpp"
#include "maths/utils.hpp"

////////////////////////////// CONSTRUCTORS

inline Matrix4::Matrix4()
    : element{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f }
{}
inline Matrix4::Matrix4(const float& i_f0, const float& i_f1, const float& i_f2, const float& i_f3,
    const float& i_f4, const float& i_f5, const float& i_f6, const float& i_f7,
    const float& i_f8, const float& i_f9, const float& i_f10, const float& i_f11,
    const float& i_f12, const float& i_f13, const float& i_f14, const float& i_f15)
    : element{ i_f0, i_f1, i_f2, i_f3,
        i_f4, i_f5, i_f6, i_f7,
        i_f8, i_f9, i_f10, i_f11,
        i_f12, i_f13, i_f14, i_f15 }
{}

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

inline Matrix4 Matrix4::Inverse() const
{
    Matrix4 R = Identity();

    float S[6];
    S[0] = element[0] * element[5] - element[4] * element[1];
    S[1] = element[0] * element[6] - element[4] * element[2];
    S[2] = element[0] * element[7] - element[4] * element[3];
    S[3] = element[1] * element[6] - element[5] * element[2];
    S[4] = element[1] * element[7] - element[5] * element[3];
    S[5] = element[2] * element[7] - element[6] * element[3];

    float C[6];
    C[0] = element[8] * element[13] - element[12] * element[9];
    C[1] = element[8] * element[14] - element[12] * element[10];
    C[2] = element[8] * element[15] - element[12] * element[11];
    C[3] = element[9] * element[14] - element[13] * element[10];
    C[4] = element[9] * element[15] - element[13] * element[11];
    C[5] = element[10] * element[15] - element[14] * element[11];

    // Assuming it is invertible
    float InvDet = 1.0f / (S[0] * C[5] - S[1] * C[4] + S[2] * C[3] + S[3] * C[2] - S[4] * C[1] + S[5] * C[0]);

    R.element[0] = +(element[5] * C[5] - element[5] * C[4] + element[7] * C[3]) * InvDet;
    R.element[1] = -(element[1] * C[5] - element[2] * C[4] + element[3] * C[3]) * InvDet;
    R.element[2] = +(element[13] * S[5] - element[14] * S[4] + element[15] * S[3]) * InvDet;
    R.element[3] = -(element[9] * S[5] - element[10] * S[4] + element[11] * S[3]) * InvDet;

    R.element[4] = -(element[4] * C[5] - element[6] * C[2] + element[7] * C[1]) * InvDet;
    R.element[5] = +(element[0] * C[5] - element[2] * C[2] + element[3] * C[1]) * InvDet;
    R.element[6] = -(element[12] * S[5] - element[14] * S[2] + element[15] * S[1]) * InvDet;
    R.element[7] = +(element[8] * S[5] - element[10] * S[2] + element[11] * S[1]) * InvDet;

    R.element[8] = +(element[4] * C[4] - element[5] * C[2] + element[7] * C[0]) * InvDet;
    R.element[9] = -(element[0] * C[4] - element[1] * C[2] + element[3] * C[0]) * InvDet;
    R.element[10] = +(element[12] * S[4] - element[13] * S[2] + element[15] * S[0]) * InvDet;
    R.element[11] = -(element[8] * S[4] - element[9] * S[2] + element[11] * S[0]) * InvDet;

    R.element[12] = -(element[4] * C[3] - element[5] * C[1] + element[6] * C[0]) * InvDet;
    R.element[13] = +(element[0] * C[3] - element[1] * C[1] + element[2] * C[0]) * InvDet;
    R.element[14] = -(element[12] * S[3] - element[13] * S[1] + element[14] * S[0]) * InvDet;
    R.element[15] = +(element[8] * S[3] - element[9] * S[1] + element[10] * S[0]) * InvDet;

    return R;
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

inline Matrix4 Matrix4::MatrixFromQuat(const Quaternion& i_q)
{
    float q0 = i_q.w, q1 = i_q.x, q2 = i_q.y, q3 = i_q.z;
    return Matrix4{
        2.f * (q0 * q0 + q1 * q1) - 1.f, 2.f * (q1 * q2 - q0 * q3), 2.f * (q1 * q3 + q0 * q2), 0.f,
        2.f * (q1 * q2 + q0 * q3), 2.f * (q0 * q0 + q2 * q2) - 1.f, 2.f * (q2 * q3 - q0 * q1), 0.f,
        2.f * (q1 * q3 - q0 * q2), 2.f * (q2 * q3 + q0 * q1), 2.f * (q0 * q0 + q3 * q3) - 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f }.Transpose();
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

inline Vector3 Matrix4::Translation() const
{
    return { element[12], element[13], element[14] };
}

inline Vector3 Matrix4::Scale() const
{
    float sx = Vector3{ element[0], element[1], element[2] }.Length();
    float sy = Vector3{ element[4], element[5], element[6] }.Length();
    float sz = Vector3{ element[8], element[9], element[10] }.Length();
    return { sx, sy, sz };
}