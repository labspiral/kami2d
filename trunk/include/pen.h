/*******************************************************************************
*	kami2d library
*
*
*	color and pen style interface
*
*
*	google code website : https://code.google.com/p/kami2d/
*	source repository : git clone https://userid@code.google.com/p/kami2d/
*	author : hong chan, choi (sepwind@gmail.com)
*
*******************************************************************************/

#ifndef penH
#define penH

#include <windows.h>
#include "config.h"


#ifdef __cplusplus
extern "C"
{
#endif

namespace kami2dlib
{

    class DLLINTERFACE Color
    {

    public:
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE a;

    public:
        DWORD colorref();

    public:
        Color();
        virtual ~Color();
    };


    // --------------------------------------------------------------------------


    class DLLINTERFACE Pen
    {

    public:

        virtual FLOAT&	width();
        virtual DWORD&	linestyle();
        virtual void 	setColor(BYTE r, BYTE g, BYTE b, BYTE alpha=255);
        virtual void 	setColor(DWORD color);
    	virtual Color&	color();

    protected:
        FLOAT	_width;
        DWORD	_linestyle;
        Color 	_color;


    public:
        Pen();
        Pen(DWORD color);
        Pen(BYTE r, BYTE g, BYTE b, BYTE alpha=255);
        virtual ~Pen();
    };


}

#ifdef __cplusplus
}
#endif

#endif
