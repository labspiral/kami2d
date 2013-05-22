#include "..\include\matrix.h"
#include <cmath>
#include <memory.h>

/*********************************************************************************

좋은 게임을 만드는 핵심 원리: 게임 수학과 물리
이상욱, 최종현, 김용준 지음 |국내서 |2003년 11월 |한빛미디어(주)
http://www.hanb.co.kr/exam/1257/

*********************************************************************************/


namespace kami2dlib
{

    #define FLOATING_POINT_ERROR_DEV	(1.0e-5)

    Matrix3::Matrix3()
    {
        // 두 구현과 동일한 의미를 가진다
        //for (int i = 0; i < 9; ++i) element[i] = 0.0;
        ::memset(element, '\0', sizeof(element));
    }

    Matrix3::Matrix3(const FLOAT rhs[9])
    {
        // 두 구현과 동일한 의미를 가진다
        //for (int i = 0; i < 9; ++i) element[i] = rhs[i];
        ::memcpy(element, rhs, sizeof(element));
    }

    Matrix3::Matrix3(const FLOAT a1, const FLOAT a2, const FLOAT a3,
            const FLOAT b1, const FLOAT b2, const FLOAT b3,
            const FLOAT c1, const FLOAT c2, const FLOAT c3)
    {
        this->element[0] = a1;
        this->element[1] = a2;
        this->element[2] = a3;
        this->element[3] = b1;
        this->element[4] = b2;
        this->element[5] = b3;
        this->element[6] = c1;
        this->element[7] = c2;
        this->element[8] = c3;
    }

    Matrix3::Matrix3(const Matrix3& rhs)
    {
        // 두 구현과 동일한 의미를 가진다
        //for (int i = 0; i < 9; ++i) element[i] = rhs.element[i];
        ::memcpy(element, rhs.element, sizeof(element));
    }

    Matrix3::~Matrix3()
    {}

    Matrix3& Matrix3::operator=(const Matrix3& rhs)
    {
        if (this == &rhs) return *this;
            ::memcpy(element, rhs.element, sizeof(element));
        return *this;
    }

    void Matrix3::Identity()
    {
        //  [식 2-6] 단위행렬
        element[0] = 1.0f;
        element[1] = 0.0f;
        element[2] = 0.0f;

        element[3] = 0.0f;
        element[4] = 1.0f;
        element[5] = 0.0f;

        element[6] = 0.0f;
        element[7] = 0.0f;
        element[8] = 1.0f;
    }

    void Matrix3::Scale(FLOAT sx, FLOAT sy)
    {
        element[0] = sx;
        element[1] = 0.0f;
        element[2] = 0.0f;

        element[3] = 0.0f;
        element[4] = sy;
        element[5] = 0.0f;

        element[6] = 0.0f;
        element[7] = 0.0f;
        element[8] = 1.0f;
    }


    void Matrix3::Rotate(FLOAT degree)
    {

        element[0] = cos( D2R(degree));
        element[1] = -sin( D2R(degree));
        element[2] = 0.0f;

        element[3] = sin( D2R(degree));
        element[4] = cos( D2R(degree));
        element[5] = 0.0f;

        element[6] = 0.0f;
        element[7] = 0.0f;
        element[8] = 1.0f;

        this->FPErrorCorrection();
    }

    void Matrix3::Translate(FLOAT dx, FLOAT dy)
    {
        element[0] = 1.0f;
        element[1] = 0.0f;
        element[2] = dx;

        element[3] = 0.0f;
        element[4] = 1.0f;
        element[5] = dy;

        element[6] = 0.0f;
        element[7] = 0.0f;
        element[8] = 1.0f;
    }


    void Matrix3::Transpose()
    {
        //  [식 2-8] 전치행렬
        Matrix3 mat(*this);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                element[j*3 + i] = mat.element[i*3 + j];
    }

    Matrix3& Matrix3::operator+()
    {  return *this;  }

    Matrix3 Matrix3::operator-() const
    {
        Matrix3 mat(*this);
        mat.Scalar(-1.0f);
        return mat;
    }

    Matrix3 Matrix3::operator+(const Matrix3& rhs) const
    {
        //  [식 2-11] 행렬의 덧셈
        Matrix3 mat(*this);
        for (int i = 0; i < 9; ++i)
            mat.element[i] += rhs.element[i];
        return mat;
    }

    Matrix3& Matrix3::operator+=(const Matrix3& rhs)
    {
        //  [식 2-11] 행렬의 덧셈
        for (int i = 0; i < 9; ++i)
            element[i] += rhs.element[i];
        return *this;
    }

    Matrix3 Matrix3::operator-(const Matrix3& rhs) const
    {
        //  [식 2-13] 행렬의 뺄셈
        Matrix3 mat(*this);
        for (int i = 0; i < 9; ++i)
            mat.element[i] -= rhs.element[i];
        return mat;
    }

    Matrix3& Matrix3::operator-=(const Matrix3& rhs)
    {
        //  [식 2-13] 행렬의 뺄셈
        for (int i = 0; i < 9; ++i)
            element[i] -= rhs.element[i];
        return *this;
    }

    Matrix3 Matrix3::operator*(const Matrix3& rhs) const
    {
        //  [식 2-15] 행렬의 곱셈
        Matrix3 mat;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    mat.element[i*3 + j] += element[i*3 + k] * rhs.element[j+ k*3];
				
        //mat.element[j*3 + i] += element[k*3 + i] * rhs.element[j*3 + k];
        return mat;
    }

    Matrix3& Matrix3::operator*=(const Matrix3& rhs)
    {
        //  [식 2-15] 행렬의 곱셈
        return (*this = *this * rhs);
    }

    void Matrix3::Scalar(FLOAT k)
    {
        //  [식 2-17] 행렬과 스칼라의 곱
        for (int i = 0; i < 9; ++i)
            element[i] *= k;
    }

    FLOAT Matrix3::Det() const
    {
        //  [식 2-22] 3차 행렬식
        FLOAT det = 0.0;
        det = element[0] * element[4] * element[8] + element[3] * element[7] * element[2] + element[6] * element[1] * element[5];
        det -= element[0] * element[7] * element[5] + element[3] * element[1] * element[8] + element[6] * element[4] * element[2];
        return det;
    }

    Matrix3 Matrix3::Inverse() const
    {
        //  [식 2-29] 역행렬
        FLOAT det = Det();

        //  행렬식이 0이면 역행렬은 존재하진 않는다(특이행렬인 경우에 해당)
        if (fabs(det) < 1.0e-5)
            return Matrix3();

        Matrix3 adjoint(Adjoint());
        adjoint.Scalar(1.0f / det);
        return adjoint;
    }

    Matrix3 Matrix3::Adjoint() const
    {
        //  [식 2-28] 수반행렬
        Matrix3 adjoint;
        adjoint.element[0] = element[4] * element[8] - element[7] * element[5];
        adjoint.element[1] = -element[1] * element[8] + element[7] * element[2];
        adjoint.element[2] = element[1] * element[5] - element[4] * element[2];

        adjoint.element[3] = -element[3] * element[8] + element[6] * element[5];
        adjoint.element[4] = element[0] * element[8] - element[6] * element[2];
        adjoint.element[5] = -element[0] * element[5] + element[3] * element[2];

        adjoint.element[6] = element[3] * element[7] - element[6] * element[4];
        adjoint.element[7] = -element[0] * element[7] + element[6] * element[1];
        adjoint.element[8] = element[0] * element[4] - element[3] * element[1];
        return adjoint;
    }

    Vector3	Matrix3::operator*(const Vector3& rhs) const
    {
        Vector3 v;
        v.x = element[0] * rhs.x + element[1] * rhs.y + element[2] * rhs.z;
        v.y = element[3] * rhs.x + element[4] * rhs.y + element[5] * rhs.z;
        v.z = element[6] * rhs.x + element[7] * rhs.y + element[8] * rhs.z;
        return v;
    }

    void  Matrix3::FPErrorCorrection()
    {
        //floating point error correction
        for (int i = 0; i < 9; ++i)
            if (fabs(element[i]) < FLOATING_POINT_ERROR_DEV)
                element[i] = 0.0f;
    }


}

