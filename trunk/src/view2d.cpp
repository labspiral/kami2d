#include "..\include\view2d.h"
#include <tchar.h>
#include <stdio.h>
#include <cassert>


namespace kami2dlib
{

    View2d::View2d(Painter2d* pPainter)
        : _hWnd(NULL)
        , _pPainter(pPainter)
    {
        assert(pPainter);
    }

    View2d::~View2d()
    {
        /// resource should be released by outside-creator
        //delete _pPainter;
        //delete _pSelection;

        _pPainter = NULL;
    }
    //---------------------------------------------------------------------------
    Painter2d* View2d::painter()
    {
        return this->_pPainter;
    }

    //---------------------------------------------------------------------------
    void View2d::initialize(HWND hWnd)
    {
        assert(hWnd);
        _pPainter->initialize(hWnd);
        _hWnd = hWnd;
    }
    //---------------------------------------------------------------------------
    void View2d::onResize(int width, int height)
    {
        if (NULL == _hWnd)
            return;

        _width = width;
        _height = height;

        _pPainter->resize( width, height);
        this->onRender();
    }

    //---------------------------------------------------------------------------
    void View2d::onZoomBegin(int x, int y, int distance)
    {
        _zoomBeginDistance = (float)distance;
        _zoomBeginScale =  _pPainter->getScale();
    }

    void View2d::onZooming(int x, int y, int distance)
    {
        FLOAT targetScale = 0.0f;

        if ( _zoomBeginDistance < distance)
        {
            targetScale = _zoomBeginScale * (float)distance / (float)_zoomBeginDistance;
        }
        else
        {
            targetScale = _zoomBeginScale / ( (float)_zoomBeginDistance / (float)distance );
        }

        this->painter()->scale( targetScale);
        this->onRender();
    }

    void View2d::onZoomEnd(int x, int y, int distance)
    {
		this->onZooming(x, y, distance);
    }
    //---------------------------------------------------------------------------
    void  View2d::onRotateBegin(int x, int y, float angle)
    {
        _rotateBeginAngle = this->painter()->getRotate();
    }

    void  View2d::onRotating(int x, int y, float angle)
    {
        this->painter()->rotate( _rotateBeginAngle + angle);
        this->onRender();

    }

    void  View2d::onRotateEnd(int x, int y, float angle)
    {
        /// permits only 90*n degree only
        FLOAT targetangle = (((int((int)fabs(_rotateBeginAngle + angle) % 360 - 45) + 90 ) / 90 ) % 4 ) * 90.0f;
        if (_rotateBeginAngle + angle < 0.0f)
            targetangle = -targetangle;

        this->painter()->rotate( targetangle);
        this->onRender();
    }
    //---------------------------------------------------------------------------
    void View2d::onPanBegin(int x, int y)
    {
        _panBeginPt.x = x;
        _panBeginPt.y = y;
    }

    void View2d::onPanning(int x, int y)
    {
        int dx = x - _panBeginPt.x;
        int dy = -(y - _panBeginPt.y);

        this->painter()->viewport(dx , dy);
        this->onRender();

        _panBeginPt.x = x;
        _panBeginPt.y = y;
    }

    void View2d::onPanEnd(int x, int y)
    {
    	this->onPanning(x, y);
    }

    void View2d::onMouseWheel(int x, int y, int delta)
    {
    	int step = delta / 120;
        float scale = this->painter()->getScale();
        float target;
        if (step > 0)
        {
        	target = scale + scale * 0.2;
        }
        else
        {
        	target = scale / (1 + 0.2);
        }

        if (target<1.0)
        	target = 1.0;

        this->painter()->scale(target);
        this->onRender();
    }

	POINT2F	View2d::onMouseMove(int x, int y)
    {
        POINT2F p = this->painter()->dp2lp(x, y);
        return p;
    }

}


