/*******************************************************************************
*	kami2d library
*
*
*	view2d interface (redraw, mouse, keyboard, touch gesture events,...)
*
*
*	google code website : https://code.google.com/p/kami2d/
*	source repository : git clone https://userid@code.google.com/p/kami2d/
*	author : hong chan, choi (sepwind@gmail.com)
*
*******************************************************************************/

#ifndef view2dH
#define view2dH

#if defined(__BORLANDC__)
	#include <vcl.h>
#endif
    
#include "painter2d.h"


#ifdef __cplusplus
extern "C"
{
#endif

namespace kami2dlib
{

    class DLLINTERFACE View2d
    {

    public:
        virtual void	initialize(HWND hWnd);

        /// multi touch gesture event
        virtual void	onZoomBegin(int x, int y, int distance);
        virtual void	onZooming(int x, int y, int distance);
        virtual void	onZoomEnd(int x, int y, int distance);
        virtual void	onRotateBegin(int x, int y, float angle);
        virtual void	onRotating(int x, int y, float angle);
        virtual void	onRotateEnd(int x, int y, float angle);
        virtual void	onPanBegin(int x, int y);
        virtual void	onPanning(int x, int y);
        virtual void	onPanEnd(int x, int y);

        /// redraw event
        virtual void	onResize(int width, int height);
        virtual DWORD	onRender()=0;


        /// keyboard event
        /// mouse event
        virtual void	onMouseWheel(int x, int y, int delta);
        virtual POINT2F	onMouseMove(int x, int y);


        /// auxillary
        Painter2d*		painter();


    protected:
        Painter2d*		_pPainter;

        HWND			_hWnd;
        INT				_width, _height;
        FLOAT			_zoomBeginDistance, _zoomBeginScale;
        FLOAT			_rotateBeginAngle;
        POINT			_panBeginPt;

    public:
        explicit View2d(Painter2d* pPainter);
        virtual ~View2d();
    };

}

#ifdef __cplusplus
}
#endif

#endif
