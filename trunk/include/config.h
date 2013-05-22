/*******************************************************************************
*	kami2d library
*
*
*	configuration
*
*
*	google code website : https://code.google.com/p/kami2d/
*	source repository : git clone https://userid@code.google.com/p/kami2d/
*	author : hong chan, choi (sepwind@gmail.com)
*
*******************************************************************************/

#ifndef _CONFIGH_
#define _CONFIGH_


/// dll defines

#if defined(KAMI2D_STATIC)
	#define DLLINTERFACE
#else

	#if defined(KAMI2D_EXPORTS)
		#define DLLINTERFACE __declspec(dllexport)
	#else
		#define DLLINTERFACE __declspec(dllimport)
	#endif

#endif


/// utility

#define D2R(degree) ((degree) / 180.0 * 3.14159265358979323846)
#define R2D(radian) ((radian) * 180.0 / 3.14159265358979323846)


#endif
