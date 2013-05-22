/*******************************************************************************
*	kami2d library
*
*
*	���� ������ ����� �ٽ� ����: ���� ���а� ����
*	�̻��, ������, ����� ���� |������ |2003�� 11�� |�Ѻ��̵��(��)
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
        // 3���� ������ ������� x,y,z
        FLOAT x;
        FLOAT y;
        FLOAT z;


        // Vector3�� ������ - ��������� �ʱ�ȭ�Ѵ�.
        Vector3();

        Vector3(FLOAT init_x, FLOAT init_y, FLOAT init_z);

        // Vector3�� ���� ������
        Vector3(const Vector3& rhs);

        // Vector3�� �Ҹ���
        virtual ~Vector3();

        // ������ ���� - ���� ���Ϳ� rhs ���Ͱ��� ������ �����Ѵ�.
        FLOAT Dot(const Vector3& rhs);

        // ������ ���� - u ���Ϳ� v ���Ͱ��� ���� ���͸� ���� ���ͷ� ����.
        //     (this = u X v )
        void Cross(const Vector3& u, const Vector3& v);

        // ���Ϳ� ��Į�� k �� ���� ���͸� �����Ѵ�.
        Vector3 Scale(FLOAT k);

        // ���� ������ ���̸� �����Ѵ�.
        FLOAT Length();

        // ���� ���Ϳ� ���� �������͸� �����Ѵ�.
        Vector3 Normalize();

        // ������ ������
        Vector3 operator+(const Vector3& rhs);		// ���纤�Ϳ� rhs������ ������ ��� ���͸� �����Ѵ�.
        Vector3 operator-(const Vector3& rhs);		// ���纤�Ϳ� rhs������ ������ ��� ���͸� �����Ѵ�.
        void operator=(const Vector3& rhs);		// rhs������ ��� ��������� ���� ���ͷ� �����Ѵ�.

    };

}

#ifdef __cplusplus
}
#endif

#endif