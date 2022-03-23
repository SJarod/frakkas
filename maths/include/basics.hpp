//
// Created by m.mehalin on 23/02/2022.
//

#pragma once

#include <cmath>

struct Vector3;
struct Vector4;

namespace Maths
{
////////////////////////////// VARIABLES

    struct Constants {
        static constexpr float pi = 3.14159265358979323846f;
        static constexpr float piHalf = pi / 2.f;
        static constexpr float degToRad = (pi / 180.f);
        static constexpr float radToDeg = (180.f / pi);
        static constexpr float epsilon = 1e-24f;
        static constexpr float weakEpsilon = 0.00001f; // Less precise but sufficient
    };

////////////////////////////// FUNCTIONS

/**
 * @summary Return minimum between two values, negative numbers accepted
 */
    template<typename T>
    T Min(T i_a, T i_b);

/**
 * @summary Return maximum between two values, negative numbers accepted
 */
    template<typename T>
    T Max(T i_a, T i_b);

/**
 * @summary Return minimum between three values, negative numbers accepted
 */
    template<typename T>
    T Min(T i_a, T i_b, T i_c);

/**
 * @summary Return maximum between three values, negative numbers accepted
 */
    template<typename T>
    T Max(T i_a, T i_b, T i_c);

/**
 * @summary Avoid a variable value to be outside of a range, negative numbers accepted
 */
    template<typename T>
    T Clamp(T i_val, T i_min, T i_max);

/**
 * @summary Return a float's absolute value
 */
    float Abs(float i_f);

/**
 * @summary Return a float's square root
 */
    float Sqrt(float i_f);

/**
 * @summary Return a float's square, negative numbers accepted
 */
    float Square(float i_f);

/**
 * @summary Return a float's cosine, negative numbers accepted
 * @param i_f   float in radian
 */
    float Cos(float i_f);

/**
 * @summary Return a float's arccosine
 * @param i_f   float in radian
 */
    float Acos(float i_f);

/**
 * @summary Return a float's sine
 * @param i_f   float in radian
 */
    float Sin(float i_f);

/**
 * @summary Return a float's arcsine
 * @param i_f   float in radian
 */
    float Asin(float i_f);

/**
 * @summary Return a float's tangent
 * @param i_f   float in radian
 */
    float Tan(float i_f);

/**
 * @summary Return a float's arctangent
 * @param i_f float in radian
 */
    float Atan(float i_f);

/**
 * @summary Linearly interpolates between i_start and i_reach by i_factor
 * @return The interpolated float result between the two float values
 * @param i_factor clamped to the range [0, 1]
 */
    template<typename T>
    T Lerp(T i_start, T i_reach, float i_factor);

/**
 * @summary Define the sign of a number, positive or negative
 * @returns 1 if the number is positive,
 *          -1 if the number is negative
 */
    template <typename T>
    int Sign(T i_val);
}

#include "basics.inl"