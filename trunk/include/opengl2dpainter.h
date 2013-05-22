/*******************************************************************************
*	kami2d library
*
*
*	opengl2d interface Simplementation
*
*
*	google code website : https://code.google.com/p/kami2d/
*	source repository : git clone https://userid@code.google.com/p/kami2d/
*	author : hong chan, choi (sepwind@gmail.com)
*
*******************************************************************************/

#ifndef opengl2dpainterH
#define opengl2dpainterH

#include "painter2d.h"
#include <gl\gl.h>
#include <gl\glu.h>


#ifdef __cplusplus
extern "C"
{
#endif

namespace kami2dlib
{

    class Opengl2dPainterPimpl;

    class DLLINTERFACE Opengl2dPainter
        : public Painter2d
    {
        public:

            virtual HWND		hwnd();
            virtual bool		initialize(HWND hWnd);
            virtual void 		resize(INT w, INT h);
            virtual void 		scale(FLOAT factor);
            virtual FLOAT		getScale();
            virtual void 		rotate(FLOAT angle);
            virtual FLOAT		getRotate();
            virtual void  		viewport(INT dx, INT dy);
            virtual POINT2F		dp2lp(POINT p);
            virtual POINT2F		dp2lp(INT x, INT y);
            virtual void		renderBegin(Pen& pen, Matrix3& m=M3_IDENTITY);
            virtual void		renderEnd();
            virtual void		drawBegin(Pen& pen, Matrix3& m=M3_IDENTITY);
            virtual void		drawEnd();
            virtual int			listBegin(Pen& pen);
            virtual void		listEnd();

            virtual	void		drawList(int index);
            virtual void		drawPoint(POINT2F& point);
            virtual void		drawPoint(FLOAT x, FLOAT y);
            virtual void		drawLine(POINT2F& p1, POINT2F& p2);
            virtual void		drawLine(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2);
            virtual void		drawTriangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3, BOOL bFill=false);
            virtual void    	drawRectangle(RECT2F& rect, BOOL bFill=false);
            virtual void		drawRectangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, BOOL bFill=false);
            virtual void		drawArc(POINT2F& ptCenter, FLOAT fRadius, FLOAT fStartAngle, FLOAT fEndAngle);
            virtual void		drawArc(FLOAT cx, FLOAT cy, FLOAT fRadius, FLOAT fStartAngle, FLOAT fEndAngle);
            virtual void		drawCircle(POINT2F& ptCenter, FLOAT fRadius, BOOL bFill=false);
            virtual void		drawCircle(FLOAT cx, FLOAT cy, FLOAT fRadius, BOOL bFill=false);
            virtual void		drawEllipse(POINT2F& ptCenter, FLOAT majorx, FLOAT minory, FLOAT fStartAngle, FLOAT fEndAngle);
            virtual void		drawEllipse(FLOAT cx, FLOAT cy, FLOAT majorx, FLOAT minory, FLOAT fStartAngle, FLOAT fEndAngle);
            virtual void		drawRoundedRect(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT radius);
            virtual void		drawRoundedRect(RECT2F& rect, FLOAT radius);
            virtual void		drawBezier(POINT2F* pPoints, INT count, INT steps=10);
            virtual void		drawPolygon(const POINT2F* pPoints, INT count, bool bFill=false);

        protected:
            virtual bool		setupPixelFormat();
            virtual bool		setupContext();
            virtual bool		initGL();
            virtual POINT2F& 	camera();

        private:
            Opengl2dPainterPimpl*	_pPimpl;

        public:
            Opengl2dPainter();
            virtual ~Opengl2dPainter();

    };

}

#ifdef __cplusplus
}
#endif

#endif
