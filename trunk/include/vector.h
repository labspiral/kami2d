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

#ifndef CVECTOR_HEADER
#define CVECTOR_HEADER

#include "config.h"
#include <windows.h>
#include <math.h>


#ifdef __cplusplus
extern "C"
{
#endif

namespace kami2dlib
{

    class DLLINTERFACE Vector3
    {
    public:
        // 3차원 벡터의 멤버변수 x,y,z
        FLOAT x;
        FLOAT y;
        FLOAT z;


        // Vector3의 생성자 - 멤버변수를 초기화한다.
        Vector3();

        Vector3(FLOAT init_x, FLOAT init_y, FLOAT init_z);

        // Vector3의 복사 생성자
        Vector3(const Vector3& rhs);

        // Vector3의 소멸자
        virtual ~Vector3();

        // 벡터의 내적 - 현재 벡터와 rhs 벡터간의 내적을 리턴한다.
        FLOAT Dot(const Vector3& rhs);

        // 벡터의 외적 - u 벡터와 v 벡터간의 외적 벡터를 현재 벡터로 대입.
        //     (this = u X v )
        void Cross(const Vector3& u, const Vector3& v);

        // 벡터에 스칼라 k 를 곱한 벡터를 리턴한다.
        Vector3 Scale(FLOAT k);

        // 현재 벡터의 길이를 리턴한다.
        FLOAT Length();

        // 현재 벡터에 대한 단위벡터를 리턴한다.
        Vector3 Normalize();

        // 연산자 재정의
        Vector3 operator+(const Vector3& rhs);		// 현재벡터와 rhs벡터의 덧셈한 결과 벡터를 리턴한다.
        Vector3 operator-(const Vector3& rhs);		// 현재벡터와 rhs벡터의 뺄셈한 결과 벡터를 리턴한다.
        void operator=(const Vector3& rhs);		// rhs벡터의 모든 멤버변수를 현재 벡터로 복사한다.

    };

}

#ifdef __cplusplus
}
#endif

#endif