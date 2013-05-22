#include "testview.h"




DWORD	TestView::onRender()
{
	DWORD dwTickCount = ::GetTickCount();

	Pen pen;
    pen.setColor(30, 30, 30);
    _pPainter->rotate( _pPainter->getRotate());
	_pPainter->renderBegin(pen);

    // draw grids
    {
        Pen pen;
        pen.setColor(50, 50, 50);

        //10mm
        _pPainter->drawBegin(pen);
        for (float i = -200; i <= 200; i+=10)
            _pPainter->drawLine( i, 200, i, -200);
        _pPainter->drawEnd();

        _pPainter->drawBegin(pen);
        for (float i = -200; i <= 200; i+=10)
            _pPainter->drawLine( -200, i, 200, i);
        _pPainter->drawEnd();

    }

    // draw axis
    {
        Pen pen;
        pen.setColor(255, 0, 0);

        _pPainter->drawBegin(pen);
        _pPainter->drawLine( -200,0, 200,0);
        _pPainter->drawEnd();

        pen.setColor(0, 255, 0);
        _pPainter->drawBegin(pen);
        _pPainter->drawLine( 0, 200, 0, -200);
        _pPainter->drawEnd();
    }

    // draw entities
    {

    	for(int i=0; i< 11; ++i)
        {
        	for(int j=0; j< 11; ++j)
            {
            	Matrix3 m;
                m.Translate( -100 +20*j, 100 - i*20);

                Pen pen(255, 255, 255);
                _pPainter->drawBegin(pen, m);
                _pPainter->drawRectangle( -7, 7, 7, -7);
                _pPainter->drawRectangle( -5, 5, 5, -5, true);
                _pPainter->drawEnd();
            }
        }
    }

	_pPainter->renderEnd();

    return ::GetTickCount() - dwTickCount;
}

