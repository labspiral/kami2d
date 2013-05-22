/*******************************************************************************
*	kami2d library
*
*
*	좋은 게임을 만드는 핵심 원리: 게임 수학과 물리
*	이상욱, 최종현, 김용준 지음 |국내서 |2003년 11월 |한빛미디어(주)
*	website : http://www.hanb.co.kr/exam/1257/
*
*	google code website : https://code.google.com/p/kami2d/
*	source repository : git clone https://userid@code.google.com/p/kami2d/
*	author : hong chan, choi (sepwind@gmail.com)
*
*******************************************************************************/

#if !defined(_CMATRIX_HEADER_)
#define _CMATRIX_HEADER_

#include "vector.h"



//  우리가 사용하는 배열, element[9]의 행렬에서의 위치는 아래와 같다
//  [  e0  e3  e6  ]
//  [  e1  e4  e7  ]
//  [  e2  e5  e8  ]


#ifdef __cplusplus
extern "C"
{
#endif

namespace kami2dlib
{

    class DLLINTERFACE Matrix3 {
    public:
        //  행렬의 생성자
        Matrix3();
        Matrix3(const FLOAT rhs[9]);
        Matrix3(const FLOAT a1, const FLOAT a2, const FLOAT a3,
                const FLOAT b1, const FLOAT b2, const FLOAT b3,
                const FLOAT c1, const FLOAT c2, const FLOAT c3);

        Matrix3(const Matrix3& rhs);
        //  행렬의 대입 연산자
        Matrix3& operator=(const Matrix3& rhs);
        //  행렬의 소멸자
        virtual ~Matrix3();

        //  단위행렬을 만든다
        void Identity();
        //  전치행렬을 만든다
        void Transpose();

        void Scale(FLOAT sx, FLOAT sy);

        void Rotate(FLOAT degree);

        void Translate(FLOAT x, FLOAT y);

        //  + 단항연산자
        Matrix3& operator+();
        //  - 단항연산자
        Matrix3 operator-() const;
        //  행렬의 덧셈
        Matrix3 operator+(const Matrix3& rhs) const;
        Matrix3& operator+=(const Matrix3& rhs);
        //  행렬의 뺄셈
        Matrix3 operator-(const Matrix3& rhs) const;
        Matrix3& operator-=(const Matrix3& rhs);
        //  행렬의 곱셈
        Matrix3 operator*(const Matrix3& rhs) const;
        Matrix3& operator*=(const Matrix3& rhs);

        Vector3 operator*(const Vector3& rhs) const;

        //  행렬과 스칼라의 곱
        void Scalar(FLOAT k);

        //  행렬식을 계산한다
        FLOAT Det() const;
        //  역행을 계산한다
        Matrix3 Inverse() const;

    protected:
        //  수반행렬을 계산한다
        Matrix3 Adjoint() const;
        void	FPErrorCorrection();

    public:
        FLOAT element[9];
    };


    static Matrix3 M3_IDENTITY(1,0,0, 0,1,0, 0,0,1);
}

#ifdef __cplusplus
}
#endif


#endif  //  _CMATRIX_HEADER_
