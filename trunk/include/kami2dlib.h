/*******************************************************************************
*	kami2d library
*
*
*
*	google code website : https://code.google.com/p/kami2d/
*	source repository : git clone https://userid@code.google.com/p/kami2d/
*	author : hong chan, choi (sepwind@gmail.com)
*
********************************************************************************
*
*
*	class diagram
*	=============
*
*
*
*	  matrix3, vector3, pen, color
*	     \
*	      \
*	       \
*			\
*	 	painter2d --------------  view2d
*		   /|\					   /|\
*	        |						|
*			|						|
*			|			            |
*	  opengl2dpainter 	      [*your viewer]
*
*
*
*
*******************************************************************************/

#ifndef KAMI2DLIBH
#define KAMI2DLIBH


#include "config.h"
#include "painter2d.h"
#include "opengl2dpainter.h"
#include "pen.h"
#include "matrix.h"
#include "vector.h"
#include "view2d.h"


#ifndef KAMI2D_STATIC
    #ifndef KAMI2D_EXPORTS
            #ifdef __BORLANDC__
                #pragma comment(lib, "kami2d.lib")
            #else
                #ifdef _DEBUG
                    #ifdef _UNICODE
                        #pragma comment(lib, "kami2dUD.lib")
                    #else
                        #pragma comment(lib, "kami2dD.lib")
                    #endif
                #else
                    #ifdef _UNICODE
                        #pragma comment(lib, "kami2dU.lib")
                    #else
                        #pragma comment(lib, "kami2d.lib")
                    #endif
                #endif
            #endif
    #endif
#endif



using namespace kami2dlib;

#endif
