#ifndef testviewH
#define testviewH

#include <windows.h>
#include "kami2dlib.h"

class TestView : public View2d
{

public:
    virtual DWORD	onRender();

private:
	Painter2d*		_pPainter;

public:
	TestView()
     : View2d(_pPainter = new Opengl2dPainter)
    {}

    virtual ~TestView()
    {
    	delete _pPainter;
        _pPainter = NULL;
    }
};


#endif
