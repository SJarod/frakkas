#include "vector3.hpp"
#include "matrix4.hpp"
#include "utils.hpp"

////////////////////////////// CONSTRUCTORS

inline Quaternion::Quaternion()
        : x(0.f), y(0.f), z(0.f), w(1.f)
{}
inline Quaternion::Quaternion(const float& _x, const float& _y, const float& _z, const float& _w)
        : x(_x), y(_y), z(_z), w(_w)
{}

////////////////////////////// OPERATORS

template<typename T>
inline Quaternion Quaternion::operator+(const T& k) const
{
    return {
            x + k,
            y + k,
            z + k,
            w + k
    };
}
inline Quaternion Quaternion::operator+(const Quaternion& q) const
{
    return {
            x + q.x,
            y + q.y,
            z + q.z,
            w + q.w
    };
}

template<typename T>
inline Quaternion Quaternion::operator-(const T& k) const
{
    return {
            x - k,
            y - k,
            z - k,
            w - k
    };
}
inline Quaternion Quaternion::operator-(const Quaternion& q) const
{
    return {
            x - q.x,
            y - q.y,
            z - q.z,
            w - q.w
    };
}

template<typename T>
inline Quaternion Quaternion::operator*(const T& k) const
{
    float qax = x, qay = y, qaz = z, qaw = w;

    return {
            qax * k + qaw * k + qay * k - qaz * k,
            qay * k + qaw * k + qaz * k - qax * k,
            qaz * k + qaw * k + qax * k - qay * k,
            qaw * k - qax * k - qay * k - qaz * k
    };
}
inline Quaternion Quaternion::operator*(const Quaternion& q) const
{
    return {
            x * q.w + y * q.z - z * q.y + w * q.x,
            -x * q.z + y * q.w + z * q.x + w * q.y,
            x * q.y - y * q.x + z * q.w + w * q.z,
            -x * q.x - y * q.y - z * q.z + w * q.w
    };
}

inline Quaternion Quaternion::operator/(const Quaternion& q) const
{
    return {
            x / q.x,
            y / q.y,
            z / q.z,
            w / q.w
    };
}

inline Quaternion Quaternion::operator*=(const Quaternion& q)
{
    return *this *= q;
}

inline bool Quaternion::operator==(const Quaternion& q) const
{
    return (
            Maths::Abs(x - q.x) <= Maths::Constants::weakEpsilon &&
            Maths::Abs(y - q.y) <= Maths::Constants::weakEpsilon &&
            Maths::Abs(z - q.z) <= Maths::Constants::weakEpsilon &&
            Maths::Abs(w - q.w) <= Maths::Constants::weakEpsilon
    );
}

inline std::ostream& operator<<(std::ostream& o, const Quaternion& q)
{
    return o << "x = " << q.x << ", y = " << q.y << ", z = " << q.z << ", w = " << q.w;
}

////////////////////////////// FUNCTIONS

inline Quaternion Quaternion::Identity()
{
    return { 0.f, 0.f, 0.f, 1.f };
}

inline Quaternion Quaternion::Normalize() const
{
    float length = this->Length();

    if (length == 0.0f)
        length = 1.0f;

    float ilength = 1.0f / length;

    return {
            x * ilength,
            y * ilength,
            z * ilength,
            w * ilength
    };
}

inline float Quaternion::Length() const
{
    return Maths::Sqrt(x * x + y * y + z * z + w * w);
}

inline Quaternion Quaternion::Invert() const
{
    Quaternion result = *this;
    float length = this->Length();
    float lengthSq = length * length;

    if (lengthSq != 0.0)
    {
        float i = 1.0f / lengthSq;

        result.x *= -i;
        result.y *= -i;
        result.z *= -i;
        result.w *= i;
    }

    return result;
}

inline Quaternion Quaternion::QuatFromMatrix(const Matrix4& i_mat)
{
    Quaternion result = Identity();

    float trace = i_mat.element[0]
                  + i_mat.element[5]
                  + i_mat.element[10];

    if (trace < 0.f)
        return Quaternion();

    result.w = Maths::Sqrt(1.f + trace) / 2.f;
    float w4 = 4.f * result.w;
    result.x = (i_mat.element[9] - i_mat.element[6]) / w4;
    result.y = (i_mat.element[2] - i_mat.element[8]) / w4;
    result.z = (i_mat.element[4] - i_mat.element[1]) / w4;

    return result;
}

inline Quaternion Quaternion::QuatFromEuler(const float& i_roll, const float& i_pitch, const float& i_yaw)
{
    float x0 = Maths::Cos(i_roll * 0.5f);
    float x1 = Maths::Sin(i_roll * 0.5f);
    float y0 = Maths::Cos(i_pitch * 0.5f);
    float y1 = Maths::Sin(i_pitch * 0.5f);
    float z0 = Maths::Cos(i_yaw * 0.5f);
    float z1 = Maths::Sin(i_yaw * 0.5f);

    return {
            x1 * y0 * z0 - x0 * y1 * z1,
            x0 * y1 * z0 + x1 * y0 * z1,
            x0 * y0 * z1 - x1 * y1 * z0,
            x0 * y0 * z0 + x1 * y1 * z1
    };
}

inline Quaternion Quaternion::QuatFromAxisAngle(const Vector3& i_axis, float i_radAngle)
{
    Quaternion result = Identity();

    if (i_axis.Length() != 0.0f)
        i_radAngle *= 0.5f;

    Vector3 newAxis = i_axis.Normalize();

    float cosres = Maths::Cos(i_radAngle);
    float sinres = Maths::Sin(i_radAngle);

    result.x = newAxis.x * sinres;
    result.y = newAxis.y * sinres;
    result.z = newAxis.z * sinres;
    result.w = cosres;

    return result.Normalize();
}

inline void Quaternion::QuatToAxisAngle(Vector3* o_outAxis, float* o_outAngle)
{
    if (Maths::Abs(w) > 1.0f)
        *this = this->Normalize();

    Vector3 resAxis = { 0.0f, 0.0f, 0.0f };
    float resAngle = 2.0f * Maths::Acos(w);
    float den = Maths::Sqrt(1.0f - w * w);

    if (den > 0.0001f)
    {
        resAxis.x = x / den;
        resAxis.y = y / den;
        resAxis.z = z / den;
    }
    else
        resAxis.x = 1.0f;

    *o_outAxis = resAxis;
    *o_outAngle = resAngle;
}

inline Quaternion Quaternion::Lerp(const Quaternion& i_q1, const Quaternion& i_q2, float i_factor)
{
    i_factor = Maths::Clamp(i_factor, 0.f, 1.f);
    return {
            i_q1.x + i_factor * (i_q2.x - i_q1.x),
            i_q1.y + i_factor * (i_q2.y - i_q1.y),
            i_q1.z + i_factor * (i_q2.z - i_q1.z),
            i_q1.w + i_factor * (i_q2.w - i_q1.w)
    };
}

inline Quaternion Quaternion::Nlerp(const Quaternion& i_q1, const Quaternion& i_q2, float i_factor)
{
    i_factor = Maths::Clamp(i_factor, 0.f, 1.f);

    Quaternion result = Lerp(i_q1, i_q2, i_factor);

    return result.Normalize();
}

inline Quaternion Quaternion::Slerp(const Quaternion& i_q1, const Quaternion& i_q2, float i_factor)
{
    i_factor = Maths::Clamp(i_factor, 0.f, 1.f);

    Quaternion result = Identity();

    float cosHalfTheta = i_q1.x * i_q2.x + i_q1.y * i_q2.y + i_q1.z * i_q2.z + i_q1.w * i_q2.w;

    if (Maths::Abs(cosHalfTheta) >= 1.0f)
        result = i_q1;
    else if (cosHalfTheta > 0.95f)
        result = Nlerp(i_q1, i_q2, i_factor);
    else
    {
        float halfTheta = Maths::Acos(cosHalfTheta);
        float sinHalfTheta = Maths::Sqrt(1.0f - cosHalfTheta * cosHalfTheta);

        if (Maths::Abs(sinHalfTheta) < 0.001f)
        {
            result.x = (i_q1.x * 0.5f + i_q2.x * 0.5f);
            result.y = (i_q1.y * 0.5f + i_q2.y * 0.5f);
            result.z = (i_q1.z * 0.5f + i_q2.z * 0.5f);
            result.w = (i_q1.w * 0.5f + i_q2.w * 0.5f);
        }
        else
        {
            float ratioA = Maths::Sin((1 - i_factor) * halfTheta) / sinHalfTheta;
            float ratioB = Maths::Sin(i_factor * halfTheta) / sinHalfTheta;

            result.x = (i_q1.x * ratioA + i_q2.x * ratioB);
            result.y = (i_q1.y * ratioA + i_q2.y * ratioB);
            result.z = (i_q1.z * ratioA + i_q2.z * ratioB);
            result.w = (i_q1.w * ratioA + i_q2.w * ratioB);
        }
    }

    return result;
}