#include "vector4.hpp"
#include "quaternion.hpp"
#include "utils.hpp"

////////////////////////////// CONSTRUCTORS

inline Vector3::Vector3()
            : x(0.f), y(0.f), z(0.f)
{}
inline Vector3::Vector3(const float& _x, const float& _y, const float& _z)
            : x(_x), y(_y), z(_z)
{}
inline Vector3::Vector3(const Vector4& v4)
            : x(v4.x), y(v4.y), z(v4.z)
{}

////////////////////////////// VARIABLES

inline const Vector3 Vector3::zero = {0.f, 0.f, 0.f};
inline const Vector3 Vector3::right = { 1.f, 0.f, 0.f};
inline const Vector3 Vector3::left = { -1.f, 0.f, 0.f};
inline const Vector3 Vector3::up = { 0.f, 1.f, 0.f};
inline const Vector3 Vector3::down = { 0.f, -1.f, 0.f};
inline const Vector3 Vector3::forward = { 0.f, 0.f, 1.f};
inline const Vector3 Vector3::backward = { 0.f, 0.f, -1.f};

////////////////////////////// OPERATORS

template<typename T>
inline Vector3 Vector3::operator*(const T& k) const
{
    return {
            x * k,
            y * k,
            z * k
    };
}
inline Vector3 Vector3::operator*(const Vector3& vec) const
{
    return {
            x * vec.x,
            y * vec.y,
            z * vec.z
    };
}

template<typename T>
inline Vector3 Vector3::operator/(const T& k) const
{
    if (k == 0.f)
        return { 0.f, 0.f, 0.f };

    T quotient = 1.f / k;

    return {
            x * quotient,
            y * quotient,
            z * quotient
    };
}
inline Vector3 Vector3::operator/(const Vector3& vec) const
{
    return {
            x / vec.x,
            y / vec.y,
            z / vec.z
    };
}

template<typename T>
inline Vector3 Vector3::operator+(const T& k) const
{
    return {
            k + x,
            k + y,
            k + z
    };
};
inline Vector3 Vector3::operator+(const Vector3& vec) const
{
    return {
            x + vec.x,
            y + vec.y,
            z + vec.z
    };
};

inline Vector3 Vector3::operator-(const Vector3& vec) const
{
    return {
            x - vec.x,
            y - vec.y,
            z - vec.z
    };
};
inline Vector3 Vector3::operator-(const float& k) const
{
    return {
            k - x,
            k - y,
            k - z
    };
};
inline Vector3 Vector3::operator-() const
{
    return {
            -x,
            -y,
            -z
    };
};

inline Vector3 Vector3::operator^(const Vector3& vec) const // Cross product
{
    return
            {
                    y * vec.z - z * vec.y,
                    z * vec.x - x * vec.z,
                    x * vec.y - y * vec.x
            };
}

inline Vector3 Vector3::operator+=(Vector3& vec)
{
    return {
            x += vec.x,
            y += vec.y,
            z += vec.z
    };
};

inline Vector3 Vector3::operator-=(Vector3& vec)
{
    return {
            x -= vec.x,
            y -= vec.y,
            z -= vec.z
    };
};

inline bool Vector3::operator==(const Vector3& vec) const
{
    return (
            Maths::Abs(x - vec.x) < Maths::Constants::weakEpsilon &&
            Maths::Abs(y - vec.y) < Maths::Constants::weakEpsilon &&
            Maths::Abs(z - vec.z) < Maths::Constants::weakEpsilon
    );
}

template<typename T>
inline Vector3& Vector3::operator*=(const T& k)
{
    return this *= k;
}

inline std::ostream& operator<<(std::ostream& o, const Vector3& vec)
{
    return o << "x = " << vec.x << ", y = " << vec.y << ", z = " << vec.z;
}

////////////////////////////// FUNCTIONS

inline Vector3 Vector3::Normalize() const
{
    float mag = this->Length();

    if (mag == 0.f)
        return *this;

    return *this / mag;
}

inline Vector3 Vector3::Abs() const
{
    return { Maths::Abs(x), Maths::Abs(y), Maths::Abs(z) };
}

inline float Vector3::Length() const
{
    return Maths::Sqrt(x * x + y * y + z * z);
}

inline float Vector3::DotProduct(const Vector3 &i_vecA, const Vector3 &i_vecB)
{
    return i_vecA.x * i_vecB.x + i_vecA.y * i_vecB.y + i_vecA.z * i_vecB.z;
};

inline Vector3 Vector3::CrossProduct(const Vector3 &i_vecA, const Vector3 &i_vecB)
{
    return	{
            i_vecA.y * i_vecB.z - i_vecA.z * i_vecB.y,
            i_vecA.z * i_vecB.x - i_vecA.x * i_vecB.z,
            i_vecA.x * i_vecB.y - i_vecA.y * i_vecB.x
    };
}

inline Vector3 Vector3::VecFromPt(const Vector3& i_ptA, const Vector3& i_ptB)
{
    return i_ptB - i_ptA;
}

inline Vector3 Vector3::RotateByQuat(const Vector3& i_v, const Quaternion& i_q)
{
    Vector3 result = zero;

    Quaternion n_q = i_q.Normalize();

    result.x = i_v.x * (n_q.w * n_q.w + n_q.x * n_q.x - n_q.y * n_q.y - n_q.z * n_q.z)	+ i_v.y * (2 * (n_q.x * n_q.y) - 2 * (n_q.w * n_q.z))					+ i_v.z * (2 * (n_q.x * n_q.z) + 2 * (n_q.w * n_q.y));
    result.y = i_v.x * (2 * (n_q.x * n_q.y) + 2 * (n_q.w * n_q.z))				+ i_v.y * (n_q.w * n_q.w + n_q.y * n_q.y - n_q.x * n_q.x - n_q.z * n_q.z)		+ i_v.z * (2 * (n_q.y * n_q.z) - 2 * (n_q.w * n_q.x));
    result.z = i_v.x * (2 * (n_q.x * n_q.z) - 2 * (n_q.w * n_q.y))				+ i_v.y * (2 * (n_q.y * n_q.z) + 2 * (n_q.w * n_q.x))					+ i_v.z * (n_q.w * n_q.w + n_q.z * n_q.z - n_q.x * n_q.x - n_q.y * n_q.y);

    return result;
}

inline Vector3 Vector3::Clamp(const Vector3& i_v, float i_min, float i_max)
{
    return Vector3(Maths::Clamp(i_v.x, i_min, i_max),
                   Maths::Clamp(i_v.y, i_min, i_max),
                   Maths::Clamp(i_v.z, i_min, i_max));
}