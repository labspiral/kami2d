/*******************************************************************************
*	kami2d library
*
*
*	painter interface (support opengl2d but possible to expand with direct2d, gdi, gdi+,...)
*
*
*	google code website : https://code.google.com/p/kami2d/
*	source repository : git clone https://userid@code.google.com/p/kami2d/
*	author : hong chan, choi (sepwind@gmail.com)
*
*******************************************************************************/

#ifndef painterH
#define painterH


#include <windows.h>
#include "pen.h"
#include "matrix.h"


#ifdef __cplusplus
extern "C"
{
#endif

namespace kami2dlib
{

    typedef struct DLLINTERFACE tagPoint2F
    {
        float x;
        float y;

        struct tagPoint2F()
            : x(0.0f)
            , y(0.0f)
        {}

        struct tagPoint2F(FLOAT x, FLOAT y)
            : x(x)
            , y(y)
        {}

    }POINT2F;


    typedef struct DLLINTERFACE tagRect2F
    {
        POINT2F leftTop;
        POINT2F rightBottom;

    }RECT2F;



    class DLLINTERFACE Painter2d
    {
        public:
            virtual HWND		hwnd()=0;
            virtual bool		initialize(HWND hWnd)=0;
            virtual void 		resize(INT w, INT h)=0;
            virtual void 		scale(FLOAT factor)=0;
            virtual FLOAT		getScale()=0;
            virtual void 		rotate(FLOAT angle)=0;
            virtual FLOAT		getRotate()=0;
            virtual void  		viewport(INT dx, INT dy)=0;
            virtual POINT2F		dp2lp(POINT p)=0;
            virtual POINT2F		dp2lp(INT x, INT y)=0;
            virtual void		renderBegin(Pen& pen, Matrix3& m=M3_IDENTITY)=0;
            virtual void		renderEnd()=0;
            virtual void		drawBegin(Pen& pen, Matrix3& m=M3_IDENTITY)=0;
            virtual void		drawEnd()=0;
            virtual int			listBegin(Pen& pen)=0;
            virtual void		listEnd()=0;

            virtual	void		drawList(int index)=0;
            virtual void		drawPoint(POINT2F& point)=0;
            virtual void		drawPoint(FLOAT x, FLOAT y)=0;
            virtual void		drawLine(POINT2F& p1, POINT2F& p2)=0;
            virtual void		drawLine(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2)=0;
            virtual void    	drawRectangle(RECT2F& rect, BOOL bFill=false)=0;
            virtual void		drawRectangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, BOOL bFill=false)=0;
            virtual void		drawTriangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3, BOOL bFill=false)=0;
            virtual void		drawArc(POINT2F& ptCenter, FLOAT fRadius, FLOAT fStartAngle, FLOAT fEndAngle)=0;
            virtual void		drawArc(FLOAT cx, FLOAT cy, FLOAT fRadius, FLOAT fStartAngle, FLOAT fEndAngle)=0;
            virtual void		drawCircle(POINT2F& ptCenter, FLOAT fRadius, BOOL bFill=false)=0;
            virtual void		drawCircle(FLOAT cx, FLOAT cy, FLOAT fRadius, BOOL bFill=false)=0;
            virtual void		drawEllipse(POINT2F& ptCenter, FLOAT majorx, FLOAT minory, FLOAT fStartAngle, FLOAT fEndAngle)=0;
            virtual void		drawEllipse(FLOAT cx, FLOAT cy, FLOAT majorx, FLOAT minory, FLOAT fStartAngle, FLOAT fEndAngle)=0;
            virtual void		drawRoundedRect(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT radius)=0;
            virtual void		drawRoundedRect(RECT2F& rect, FLOAT radius)=0;
            virtual void		drawBezier(POINT2F* pPoints, INT count, INT steps=10)=0;
            virtual void		drawPolygon(const POINT2F* pPoints, INT count, bool bFill=false)=0;

    };

}

#ifdef __cplusplus
}
#endif

#endif

