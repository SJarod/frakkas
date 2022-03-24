#include <iostream>

#include <iostream>

#include "maths.hpp"

//#define LOG_TEST
#define LOG_ASSERTION

// Test des vecteurs : https://physics.icalculator.info/vector-calculator/addition-and-subtraction-of-vectors.html
// Test des quaternions : https://www.energid.com/resources/orientation-calculator
//                        https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
// Test des matrices : https://matrixcalc.org/en/
//                     https://www.redcrab-software.com/en/Calculator/3x3/Matrix/Rotation-XYZ
//                     Projet 'ibr' de Paul (pour la projection)

void TestVector2()
{
    std::cout << "\n////////////////////////////// VECTOR2" << "\n\n";

    Vector2 test_constructor1;
    Vector2 test_constructor2(1.f, 2.f);

    Vector2 test(-2.f, 4.f);

#ifdef LOG_TEST
    std::cout << "test_constructor1 : " << test_constructor1 << '\n';
    std::cout << "test_constructor2 : " << test_constructor2 << '\n';

    std::cout << "\ntest_vector : " << test << '\n';

    std::cout << "\ntest_normalize : " << test.Normalize() << '\n';
    std::cout << "test_abs : " << test.Abs() << '\n';
    std::cout << "test_length = " << test.Length() << '\n';
#endif
}

void TestVector3()
{
    std::cout << "\n////////////////////////////// VECTOR3" << "\n\n";

    Vector3 test_constructor1;
    Vector3 test_constructor2(1.f, 2.f, 3.f);
    Vector3 test_constructor3(Vector4(1.f, 2.f, 3.f, 1.f));

    Vector3 test_zero = Vector3::zero;
    Vector3 test_right = Vector3::right;
    Vector3 test_left = Vector3::left;
    Vector3 test_up = Vector3::up;
    Vector3 test_down = Vector3::down;
    Vector3 test_forward = Vector3::forward;
    Vector3 test_backward = Vector3::backward;

    Maths::Constants::piHalf;

    Vector3 test(-1.f, 1.f, 0.f);
    Vector3 test2(0.f, 1.f, 1.f);

    Vector3 test3(1, 0, 0);
    Quaternion quat = Quaternion::QuatFromAxisAngle(Vector3(0, 0, 1), Maths::Constants::piHalf);

    float test_dP = Vector3::DotProduct(test, test2);
    Vector3 test_cP = Vector3::CrossProduct(test, test2);
    Vector3 test_vP = Vector3::VecFromPt(test, test2);

    Vector3 test_rQ = Vector3::RotateByQuat(test3, quat);

#ifdef LOG_TEST
    std::cout << "test_constructor1 : " << test_constructor1 << '\n';
    std::cout << "test_constructor2 : " << test_constructor2 << '\n';
    std::cout << "test_constructor3 : " << test_constructor3 << '\n';

    std::cout << "\ntest_zero : " << test_zero << '\n';
    std::cout << "test_right : " << test_right << '\n';
    std::cout << "test_left : " << test_left << '\n';
    std::cout << "test_up : " << test_up << '\n';
    std::cout << "test_down : " << test_down << '\n';
    std::cout << "test_forward : " << test_forward << '\n';
    std::cout << "test_backward : " << test_backward << '\n';

    std::cout << "test_vector : " << test << '\n';

    std::cout << "\ntest_normalize : x = " << test.Normalize() << '\n';
    std::cout << "test_abs : x = " << test.Abs() << '\n';
    std::cout << "test_length = " << test.Length() << '\n';

    std::cout << "\ntest_vector2 : " << test2 << '\n';

    std::cout << "\ntest_dotProduct = " << test_dP << '\n';
    std::cout << "test_crossProduct : " << test_cP << '\n';
    std::cout << "test_vecPt : " << test_vP << '\n';

    std::cout << std::endl << "test_vector3 : " << test3 << std::endl;
    std::cout << std::endl << "quaternion : " << quat << std::endl;
    std::cout << "test_rotQuat : " << test_rQ << '\n';
#endif

#ifdef LOG_ASSERTION
    if (test_dP != 1)
        std::cout << "ERROR : dot product is not correct" << std::endl;

    if (test_cP != Vector3(1, 1, -1))
        std::cout << "ERROR : cross product is not correct" << std::endl;

    if (test_vP != Vector3(1, 0, 1))
        std::cout << "ERROR : difference between vectors is not correct" << std::endl;

    if (test_rQ != Vector3(0.f, 1.f, 0.f))
        std::cout << "ERROR : vector rotation by quaternion is not correct" << std::endl;
#endif
}

void TestVector4()
{
    std::cout << "\n////////////////////////////// VECTOR4" << "\n\n";

    Vector4 test_constructor1;
    Vector4 test_constructor2(1.f, 2.f, 3.f, 0.f);
    Vector4 test_constructor3(Vector3(1.f, 2.f, 3.f), 1.f);

    Vector4 test(-2.f, 4.f, -6.f, 5.0f);

#ifdef LOG_TEST
    std::cout << "test_constructor1 : " << test_constructor1 << '\n';
    std::cout << "test_constructor2 : " << test_constructor2 << '\n';
    std::cout << "test_constructor3 : " << test_constructor3 << '\n';

    std::cout << "\ntest_vector : " << test << '\n';

    std::cout << "\ntest_normalize : " << test.Normalize() << '\n';
    std::cout << "test_abs : " << test.Abs() << '\n';
    std::cout << "test_length = " << test.Length() << '\n';
#endif
}

void TestQuaternion()
{
    std::cout << "\n////////////////////////////// QUATERNION" << "\n\n";

    Quaternion test_constructor1;
    Quaternion test_constructor2(1.f, 2.f, 3.f, 4.f);

    Quaternion quat(0.f, 1.f, 0.f, 1.f);

    Matrix4 mat = {
            0.f, 0.f, 1.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            -1.f, 0.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 0.f
    };
    Quaternion quatMat = Quaternion::QuatFromMatrix(mat);
    Quaternion quatEuler = Quaternion::QuatFromEuler(0.f, Maths::Constants::pi, 0.f);
    Quaternion quatFromAxis = Quaternion::QuatFromAxisAngle(Vector3(0.f, 1.f, 0.f), Maths::Constants::piHalf);

    Quaternion qStart = Quaternion(0.f, 0.f, 0.f, 1.f);
    Quaternion qEnd = Quaternion(0.f, 1.f, 0.f, 1.f);
    qEnd = qEnd.Normalize();
    Quaternion qLerp = quat.Lerp(qStart, qEnd, 0.5f);
    Quaternion qNLerp = quat.Nlerp(qStart, qEnd, 0.5f);
    Quaternion qSLerp = quat.Slerp(qStart, qEnd, 0.5f);

#ifdef LOG_TEST
    std::cout << "test_constructor1 : " << test_constructor1 << '\n';
    std::cout << "test_constructor2 : " << test_constructor2 << '\n';
    std::cout << "test_identity : " << Quaternion::Identity() << '\n';

    std::cout << std::endl << "example quaternion : " << quat << std::endl;
    std::cout << "test_normalize : " << quat.Normalize() << '\n';
    std::cout << "test_length : " << quat.Length() << '\n';
    std::cout << "test_invert : " << quat.Invert() << '\n';

    std::cout << "\ntest_fromMat : " << quatMat << '\n';
    std::cout << "test_fromEuler : " << quatEuler << '\n';
    std::cout << "test_fromAxis : " << quatFromAxis << '\n';

    std::cout << "\nstarting quaternion : " << qStart << '\n';
    std::cout << "Ending quaternion : " << qEnd << '\n';
    std::cout << "test_lerp : " << qLerp << '\n';
    std::cout << "test_Nlerp : " << qNLerp << '\n';
    std::cout << "test_Slerp : " << qSLerp << '\n';
#endif

#ifdef LOG_ASSERTION
    if (quat.Normalize() != Quaternion(0.f, 0.7071067811865475f, 0.f, 0.7071067811865475f))
        std::cout << "ERROR : Quaternion normalization is not correct" << std::endl;
    if (quat.Invert() != Quaternion(0.f, -0.5f, 0.f, 0.5f))
        std::cout << "ERROR : Quaternion inversion is not correct" << std::endl;

    if (quatMat != Quaternion(0.f, 0.707107, 0.f, 0.707107f))
        std::cout << "ERROR : Quaternion from Matrix is not correct" << std::endl;
    if (quatEuler != Quaternion(0.f, 1.f, 0.f, 0.f))
        std::cout << "ERROR : Quaternion from Euler is not correct" << std::endl;
    if (quatFromAxis != Quaternion(0.f, 0.707107, 0.f, 0.707107))
        std::cout << "ERROR : Quaternion from Axis-Angle is not correct" << std::endl;

    if (qSLerp != Quaternion(0.f, 0.38268352626028224f, 0.f, 0.9238795917217785f))
        std::cout << "ERROR : SLerp is not correct" << std::endl;
#endif
}

void TestMatrix4()
{
    std::cout << "\n////////////////////////////// MATRIX4" << "\n\n";

    Matrix4 mat = Matrix4::Identity();
    Vector3 v1(1.f, 2.f, 3.f);

    float angle = Maths::Constants::piHalf;
    Vector3 v2(angle, angle, angle);

    Matrix4 matXRot = mat.RotateX(angle);
    Matrix4 matYRot = mat.RotateY(angle);
    Matrix4 matZRot = mat.RotateZ(angle);
    Matrix4 matXYZRot = mat.RotateXYZ(v2);

    Matrix4 m1 = {
        2.f, 0.f, 0.f, 0.f,
        0.f, 2.f, 0.f, 1.f,
        1.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    };

    Matrix4 m2 = {
        0.f, 0.f, 1.f, 1.f,
        0.f, 1.f, 1.f, 0.f,
        1.f, 1.f, 0.f, 0.f,
        1.f, 0.f, 0.f, 0.f
    };

    Matrix4 m1xm2 = m1 * m2;

    float fov = 90.f; // WARNING: The fov should be in radians, but for tests we don't take care about it

    Matrix4 projection = Matrix4::Perspective(fov, 16.f/9.f, 0.001f, 1000.f, false);
    Matrix4 orthographic = Matrix4::Perspective(fov, 16.f/9.f, 0.001f, 1000.f, true);

#ifdef LOG_TEST
    std::cout << "test_identity : \n" << mat << '\n';
    std::cout << "test_null : \n" << Matrix4::Null() << '\n';
    std::cout << "test_multiplyVector : " << mat * v1 << '\n';

    //std::cout << "test_frustum : \n" << mat.Frustum(left, right, bottom, top, near, far) << '\n';
    //std::cout << "test_ortho : \n" << mat.Orthographic(left, right, bottom, top, near, far) << '\n';
    //std::cout << "test_perspective : \n" << mat.Perspective(fov, aspect, near, far, false) << '\n';

    std::cout << std::endl << "test_multiplication m1 : \n" << m1 << std::endl;
    std::cout << "test_multiplication m2 : \n" << m1 << std::endl;
    std::cout << "test_multiplication : \n" << m1xm2 << std::endl;

    std::cout << std::endl << "test_rotateX : \n" << matXRot << '\n';
    std::cout << "test_rotateY : \n" << matYRot << '\n';
    std::cout << "test_rotateZ : \n" << matZRot << '\n';
    std::cout << "test_rotateXYZ : \n" << matXYZRot << '\n';

    std::cout << "test_translate1 : \n" << mat.Translate(1.f, 2.f, 3.f) << '\n';
    std::cout << "test_translate2 : \n" << mat.Translate(v2) << '\n';

    std::cout << "test_scale1 : \n" << mat.Scale(angle) << '\n';
    std::cout << "test_scale2 : \n" << mat.Scale(v2) << '\n';

    std::cout << "\n test_perspective : \n" << projection << std::endl;
    std::cout << "test_orthographic : \n" << orthographic << std::endl;
#endif
#ifdef LOG_ASSERTION
    Matrix4 multiplyAssert = {
            0.f, 0.f, 2.f, 2.f,
            1.f, 2.f, 2.f, 0.f,
            0.f, 0.f, 1.f, 1.f,
            1.f, 0.f, 0.f, 0.f
    };

    Matrix4 xAssert = {
            1.f, 0.f, 0.f, 0.f,
            0.f, 0.f,-1.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 1.f,
    };
    Matrix4 yAssert = {
            0.f, 0.f, 1.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            -1.f, 0.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 1.f,
    };
    Matrix4 zAssert = {
            0.f,-1.f, 0.f, 0.f,
            1.f, 0.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f,
    };
    Matrix4 xyzAssert = {
            1.f, 0.f, 0.f, 0.f,
            0.f, 0.f,-1.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 1.f
    };

    Matrix4 projAssert = {
            0.61737f,   0.f,        0.f,    0.f,
            0.f,        0.61737,    0.f,    0.f,
            0.f,        0.f,        -1.f,   -1.f,
            0.f,        0.f,        -0.002f, 0.f
    };

    Matrix4 orthoAssert = {
            617.36962f, 0.f,        0.f,        0.f,
            0.f,        617.36962f, 0.f,        0.f,
            0.f,        0.f,        -0.002f,    0.f,
            0.f,        0.f,        -1.f,       0.f
    };

    if (multiplyAssert != m1xm2)
        std::cout << "ERROR : Matrice multiplication is not correct" << std::endl;

    if (matXRot != xAssert)
        std::cout << "ERROR : Matrix X Rotation is not correct" << std::endl;
    if (matYRot != yAssert)
        std::cout << "ERROR : Matrix Y Rotation is not correct" << std::endl;
    if (matZRot != zAssert)
        std::cout << "ERROR : Matrix Z Rotation is not correct" << std::endl;
    if (matXYZRot != xyzAssert)
        std::cout << "ERROR : Matrix XYZ Rotation is not correct" << std::endl;

    if (projection != projAssert)
    {
        std::cout << "ERROR : Projection Matrix from frustrum is not correct" << std::endl;
        std::cout << "--- Attempted matrix is : \n" << projAssert << std::endl;
    }

    if (orthographic != orthoAssert)
    {
        std::cout << "ERROR : Projection Matrix from orthographic is not correct" << std::endl;
        std::cout << "--- Attempted matrix is : \n" << orthoAssert << std::endl;
    }

#endif
}

void TestReferential()
{
    std::cout << "\n////////////////////////////// REFERENTIAL" << "\n\n";

    Referential test_constructor1;
    Referential test_constructor2(Vector3(1.f, 2.f, 3.f), Quaternion::Identity());
    Referential test_constructor3(Vector3(1.f, 2.f, 3.f), Vector3(4.f, 5.f, 6.f));
    Referential test_constructor4(Vector3(1.f, 2.f, 3.f), Vector3(8.f, 3.f, 0.f),
                                      Vector3(0.f, 7.f, 5.f), Vector3(5.f, 2.f, 7.f));

    Referential test_ref(Vector3(1.f, 2.f, 3.f), Vector3(4.f, 5.f, 6.f));
    Vector3 local(5.f, 7.f, 1.f);
    Vector3 global(0.f, 2.f, 4.f);

#ifdef LOG_TEST
    std::cout << "test_constructor1 : " << test_constructor1 << '\n';
    std::cout << "\ntest_constructor2 : " << test_constructor2 << '\n';
    std::cout << "\ntest_constructor3 : " << test_constructor3 << '\n';
    std::cout << "\ntest_constructor4 : " << test_constructor4 << '\n';

    std::cout << "\ntest_ref : " << test_ref << '\n';


    std::cout << "\ntest_posLocToGlob : " << test_ref.PosLocalToGlobal(local) << '\n';
    std::cout << "test_posGlobToLoc : " << test_ref.PosGlobalToLocal(global) << '\n';
    std::cout << "test_vecLocToGlob : " << test_ref.VectLocalToGlobal(local) << '\n';
    std::cout << "test_vecGlobToLoc : " << test_ref.VectGlobalToLocal(global) << '\n';
#endif
}

int main()
{
    auto testCode = 0b111111;

    if (testCode & 0b100000)
        TestVector2();
    if (testCode & 0b010000)
        TestVector3();
    if (testCode & 0b001000)
        TestVector4();
    if (testCode & 0b000100)
        TestQuaternion();
    if (testCode & 0b000010)
        TestMatrix4();
    if (testCode & 0b000001)
        TestReferential();

    getchar();

    return 0;
}