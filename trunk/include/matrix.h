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

#if !defined(_CMATRIX_HEADER_)
#define _CMATRIX_HEADER_

#include "vector.h"



//  �츮�� ����ϴ� �迭, element[9]�� ��Ŀ����� ��ġ�� �Ʒ��� ����
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
        //  ����� ������
        Matrix3();
        Matrix3(const FLOAT rhs[9]);
        Matrix3(const FLOAT a1, const FLOAT a2, const FLOAT a3,
                const FLOAT b1, const FLOAT b2, const FLOAT b3,
                const FLOAT c1, const FLOAT c2, const FLOAT c3);

        Matrix3(const Matrix3& rhs);
        //  ����� ���� ������
        Matrix3& operator=(const Matrix3& rhs);
        //  ����� �Ҹ���
        virtual ~Matrix3();

        //  ��������� �����
        void Identity();
        //  ��ġ����� �����
        void Transpose();

        void Scale(FLOAT sx, FLOAT sy);

        void Rotate(FLOAT degree);

        void Translate(FLOAT x, FLOAT y);

        //  + ���׿�����
        Matrix3& operator+();
        //  - ���׿�����
        Matrix3 operator-() const;
        //  ����� ����
        Matrix3 operator+(const Matrix3& rhs) const;
        Matrix3& operator+=(const Matrix3& rhs);
        //  ����� ����
        Matrix3 operator-(const Matrix3& rhs) const;
        Matrix3& operator-=(const Matrix3& rhs);
        //  ����� ����
        Matrix3 operator*(const Matrix3& rhs) const;
        Matrix3& operator*=(const Matrix3& rhs);

        Vector3 operator*(const Vector3& rhs) const;

        //  ��İ� ��Į���� ��
        void Scalar(FLOAT k);

        //  ��Ľ��� ����Ѵ�
        FLOAT Det() const;
        //  ������ ����Ѵ�
        Matrix3 Inverse() const;

    protected:
        //  ��������� ����Ѵ�
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
