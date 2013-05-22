#include "..\include\vector.h"

/*********************************************************************************

���� ������ ����� �ٽ� ����: ���� ���а� ����
�̻��, ������, ����� ���� |������ |2003�� 11�� |�Ѻ��̵��(��)
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


    // ������ ���� - ���� ���Ϳ� rhs ���Ͱ��� ������ �����Ѵ�.
    FLOAT Vector3::Dot(const Vector3& rhs)
    {
        return (x * rhs.x + y * rhs.y + z * rhs.z);
    }


    // ������ ���� - u ���Ϳ� v ���Ͱ��� ���� ���͸� ���� ���ͷ� ����.
    //     (this = u X v )
    void Vector3::Cross(const Vector3& u, const Vector3& v)
    {
	
        x = u.y * v.z - u.z * v.y;
        y = u.z * v.x - u.x * v.z;
        z = u.x * v.y - u.y * v.x;
    }

    // ���Ϳ� ��Į�� k �� ���� ���͸� �����Ѵ�.
    Vector3 Vector3::Scale(FLOAT k)
    {
        return Vector3(k*x, k*y, k*z);
    }


    // ���� ������ ���̸� �����Ѵ�.
    FLOAT Vector3::Length()
    {
        return FLOAT(sqrt(x*x + y*y + z*z));
    }

    // ���� ���Ϳ� ���� �������͸� �����Ѵ�.
    Vector3 Vector3::Normalize()
    {
        return Scale(1.0f/Length());
    }

    // ������ ������
    // ���纤�Ϳ� rhs������ ������ ��� ���͸� �����Ѵ�.
    Vector3 Vector3::operator+(const Vector3& rhs)
    {
        return Vector3(x+rhs.x , y+rhs.y , z+rhs.z);
    }


    // ���纤�Ϳ� rhs������ ������ ��� ���͸� �����Ѵ�.
    Vector3 Vector3::operator-(const Vector3& rhs)
    {
        return Vector3(x-rhs.x , y-rhs.y , z-rhs.z);
    }

    // rhs������ ��� ��������� ���� ���ͷ� �����Ѵ�.
    void Vector3::operator=(const Vector3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
    }

}
