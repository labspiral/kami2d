#include "..\include\vector.h"

/*********************************************************************************

좋은 게임을 만드는 핵심 원리: 게임 수학과 물리
이상욱, 최종현, 김용준 지음 |국내서 |2003년 11월 |한빛미디어(주)
http://www.hanb.co.kr/exam/1257/

*********************************************************************************/


namespace kami2dlib
{


    Vector3::Vector3()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    Vector3::Vector3(FLOAT init_x, FLOAT init_y, FLOAT init_z)
    {
        x = init_x;
        y = init_y;
        z = init_z;
    }

    Vector3::Vector3(const Vector3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
    }


    Vector3::~Vector3()
    {
	
    }


    // 벡터의 내적 - 현재 벡터와 rhs 벡터간의 내적을 리턴한다.
    FLOAT Vector3::Dot(const Vector3& rhs)
    {
        return (x * rhs.x + y * rhs.y + z * rhs.z);
    }


    // 벡터의 외적 - u 벡터와 v 벡터간의 외적 벡터를 현재 벡터로 대입.
    //     (this = u X v )
    void Vector3::Cross(const Vector3& u, const Vector3& v)
    {
	
        x = u.y * v.z - u.z * v.y;
        y = u.z * v.x - u.x * v.z;
        z = u.x * v.y - u.y * v.x;
    }

    // 벡터에 스칼라 k 를 곱한 벡터를 리턴한다.
    Vector3 Vector3::Scale(FLOAT k)
    {
        return Vector3(k*x, k*y, k*z);
    }


    // 현재 벡터의 길이를 리턴한다.
    FLOAT Vector3::Length()
    {
        return FLOAT(sqrt(x*x + y*y + z*z));
    }

    // 현재 벡터에 대한 단위벡터를 리턴한다.
    Vector3 Vector3::Normalize()
    {
        return Scale(1.0f/Length());
    }

    // 연산자 재정의
    // 현재벡터와 rhs벡터의 덧셈한 결과 벡터를 리턴한다.
    Vector3 Vector3::operator+(const Vector3& rhs)
    {
        return Vector3(x+rhs.x , y+rhs.y , z+rhs.z);
    }


    // 현재벡터와 rhs벡터의 뺄셈한 결과 벡터를 리턴한다.
    Vector3 Vector3::operator-(const Vector3& rhs)
    {
        return Vector3(x-rhs.x , y-rhs.y , z-rhs.z);
    }

    // rhs벡터의 모든 멤버변수를 현재 벡터로 복사한다.
    void Vector3::operator=(const Vector3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
    }

}
