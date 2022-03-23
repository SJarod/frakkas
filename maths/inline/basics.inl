#include "vector3.hpp"
#include "vector4.hpp"

namespace Maths {
    template<typename T>
    inline T Min(T i_a, T i_b)
    {
        return i_a < i_b ? i_a : i_b;
    }

    template<typename T>
    inline T Max(T i_a, T i_b)
    {
        return i_a > i_b ? i_a : i_b;
    }

    template<typename T>
    inline T Min(T i_a, T i_b, T i_c)
    {
        return Min(Min(i_a, i_b), i_c);
    }

    template<typename T>
    inline T Max(T i_a, T i_b, T i_c)
    {
        return Max(Max(i_a, i_b), i_c);
    }

    template<typename T>
    inline T Clamp(T i_val, T i_min, T i_max)
    {
        return Max(Min(i_val, i_max), i_min);
    }

    inline Vector3 Clamp(Vector3 i_val, float i_min, float i_max)
    {
        return Vector3(Clamp(i_val.x, i_min, i_max), Clamp(i_val.y, i_min, i_max), Clamp(i_val.z, i_min, i_max));
    }

    inline Vector4 Clamp(Vector4 i_val, float i_min, float i_max)
    {
        return Vector4(Clamp(i_val.x, i_min, i_max), Clamp(i_val.y, i_min, i_max), Clamp(i_val.z, i_min, i_max), Clamp(i_val.w, i_min, i_max));
    }

    inline float Abs(float i_f)
    {
        return fabs(i_f);
    }

    inline float Sqrt(float i_f)
    {
        return sqrtf(i_f);
    }

    inline float Square(float i_f)
    {
        return i_f * i_f;
    }

    inline float Cos(float i_f)
    {
        return cosf(i_f);
    }

    inline float Acos(float i_f)
    {
        return acosf(i_f);
    }

    inline float Sin(float i_f)
    {
        return sinf(i_f);
    }

    inline float Asin(float i_f)
    {
        return asinf(i_f);
    }

    inline float Tan(float i_f)
    {
    return tanf(i_f);
    }

    inline float Atan(float i_f)
    {
        return atanf(i_f);
    }

    template<typename T>
    inline T Lerp(T i_start, T i_reach, float i_factor)
    {
        i_factor = Clamp(i_factor, 0.f, 1.f);
        return i_start + i_factor * (i_reach - i_start);
    }

    template<typename T>
    inline int Sign(T i_val)
    {
        return (T(0) < i_val) - (i_val < T(0));
    }
}