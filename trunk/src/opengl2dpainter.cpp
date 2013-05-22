#include "..\include\opengl2dpainter.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <tchar.h>
#include <float.h>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cassert>
#include <algorithm>


namespace kami2dlib
{

    #define	GL_SELECTION_MAX	(512*512)

    class OpenglContext
    {
    private:
        BOOL	_bDiff;
        HDC 	_hdc;
        HGLRC	_hrc;

    public:

        explicit OpenglContext(HDC hdc, HGLRC hrc)
            : _bDiff(FALSE)
            , _hdc(NULL)
            , _hrc(NULL)
        {
            _bDiff |=  wglGetCurrentContext() != hrc;
            _bDiff |=  wglGetCurrentDC() != hdc;

            if (_bDiff)
            {
                _hdc = wglGetCurrentDC();
                _hrc = wglGetCurrentContext();
                wglMakeCurrent(hdc, hrc);
            }
        }

        virtual ~OpenglContext()
        {
            if (_bDiff)
            {
                /// to speed up
                //wglMakeCurrent(_hdc, _hrc);
            }
        }
    };



    void __stdcall CustomTessBegin(GLenum prim)
    {
        glBegin(prim);
    }
    void __stdcall CustomTessVertex(void *data)
    {
        glVertex3dv((GLdouble *)data);
    }
    void __stdcall CustomTessEnd()
    {
        glEnd();
    }
    void __stdcall CustomTessError(GLenum errorCode)
    {

    }


    class Opengl2dPainterPimpl
    {
    public:

        HWND				hWnd;
        HDC					hdc;
        HGLRC				hrc;
        FLOAT				width, height;
        FLOAT				scale;
        FLOAT				angle;
        POINT2F				camera;
        Pen 				pen;
        Matrix3				worldMatrix;
        Matrix3				localMatrix;
        std::set<int>		displaylists;

        GLUtriangulatorObj*	pTesslation;

        Opengl2dPainterPimpl()
            : hWnd(NULL)
            , hdc(NULL)
            , width(0)
            , height(0)
            , scale(1.0f)
            , angle(0.0f)
            , pTesslation(NULL)

        {
            worldMatrix = M3_IDENTITY;
            localMatrix = M3_IDENTITY;
        }

        virtual ~Opengl2dPainterPimpl(){}
    };



    Opengl2dPainter::Opengl2dPainter()
    {
        _pPimpl = new Opengl2dPainterPimpl;
    }

    Opengl2dPainter::~Opengl2dPainter()
    {
        /// clear display lsits
        std::set<int>::iterator it;
        it = _pPimpl->displaylists.begin();
        for ( ; it != _pPimpl->displaylists.end(); it++)
        {
            glDeleteLists(*it, 1);
        }
        _pPimpl->displaylists.clear();


        /// clear tesslation
        if (_pPimpl->pTesslation)
            gluDeleteTess(_pPimpl->pTesslation);

        if (_pPimpl->hrc)
        {
            if (!wglMakeCurrent(NULL,NULL))
            {
                ::MessageBox(NULL, _T("Release Of DC And RC Failed."), _T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
            }

            if (!wglDeleteContext(_pPimpl->hrc))
            {
                ::MessageBox(NULL, _T("Release Rendering Context Failed."), _T("SHUTDOWN ERROR") ,MB_OK | MB_ICONINFORMATION);
            }
            _pPimpl->hrc=NULL;
        }

        if (_pPimpl->hdc && !::ReleaseDC(_pPimpl->hWnd, _pPimpl->hdc))
        {
            ::MessageBox(NULL, _T("Release Device Context Failed."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
            _pPimpl->hdc = NULL;
        }

        _pPimpl->hWnd = NULL;

        delete _pPimpl;
        _pPimpl = NULL;
    }


    HWND  Opengl2dPainter::hwnd()
    {
        return _pPimpl->hWnd;
    }

    bool	Opengl2dPainter::initialize(HWND hWnd)
    {
    	///disable float operation exceptions
    	_control87(MCW_EM, MCW_EM);

        assert( NULL == _pPimpl->hWnd);
        _pPimpl->hWnd = hWnd;
        _pPimpl->hdc = ::GetDC( _pPimpl->hWnd );
        assert(_pPimpl->hdc);

        bool bInit =	\
                this->setupPixelFormat()&&
                this->setupContext() 	&&
                this->initGL()			;

        return bInit;
    }


    FLOAT Opengl2dPainter::getScale()
    {
        return this->_pPimpl->scale;
    }


    FLOAT Opengl2dPainter::getRotate()
    {
        return this->_pPimpl->angle;
    }


    bool Opengl2dPainter::setupPixelFormat()
    {

        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR), 	// Size Of This Pixel Format Descriptor
            1, 								// Version Number
            PFD_DRAW_TO_WINDOW | 			// Format Must Support Window
            PFD_SUPPORT_OPENGL | 			// Format Must Support OpenGL
            PFD_DOUBLEBUFFER, 				// Must Support Double Buffering
            PFD_TYPE_RGBA, 					// Request An RGBA Format
            32, 							// Select Our Color Depth
            0, 0, 0, 0, 0, 0, 				// Color Bits Ignored
            0, 								// No Alpha Buffer
            0, 								// Shift Bit Ignored
            0, 								// No Accumulation Buffer
            0, 0, 0, 0, 					// Accumulation Bits Ignored
            16, 							// 16Bit Z-Buffer (Depth Buffer)
            0, 								// No Stencil Buffer
            0, 								// No Auxiliary Buffer
            PFD_MAIN_PLANE, 				// Main Drawing Layer
            0, 								// Reserved
            0, 0, 0 						// Layer Masks Ignored
        };


        GLuint pixelFormat = ::ChoosePixelFormat(_pPimpl->hdc, &pfd);
        if (!pixelFormat)
        {
            ::MessageBox(NULL, _T( "Can't Find A Suitable PixelFormat."), _T("ERROR") , MB_OK|MB_ICONEXCLAMATION);
            return false;
        }

        if (!::SetPixelFormat(_pPimpl->hdc, pixelFormat, &pfd))
        {
            ::MessageBox(NULL, _T("Can't Set The PixelFormat."), _T("ERROR"), MB_OK|MB_ICONEXCLAMATION);
            return false;
        }

        return true;
    }


    bool Opengl2dPainter::	setupContext()
    {
        if(!(_pPimpl->hrc = (HGLRC)::wglCreateContext(_pPimpl->hdc)))
        {
            MessageBox(NULL, _T("Can't Create A GL Rendering Context."), _T("ERROR"), MB_OK|MB_ICONEXCLAMATION);
            return false;
        }


        if(!wglMakeCurrent(_pPimpl->hdc, _pPimpl->hrc))
        {
            MessageBox(NULL, _T("Can't Activate The GL Rendering Context."), _T("ERROR"), MB_OK|MB_ICONEXCLAMATION);
            return false;
        }

        return true;
    }


    bool Opengl2dPainter::initGL()
    {
        glClearColor(0, 0, 0, 1);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        glShadeModel(GL_SMOOTH);

        _pPimpl->pTesslation = gluNewTess();

        gluTessCallback(_pPimpl->pTesslation, GLU_TESS_BEGIN,
            (void (__stdcall *)(void))CustomTessBegin); //<1>
        gluTessCallback(_pPimpl->pTesslation, GLU_TESS_VERTEX,
            (void (__stdcall *)(void))CustomTessVertex); //<2>
        gluTessCallback(_pPimpl->pTesslation, GLU_TESS_END,
            (void (__stdcall *)(void))CustomTessEnd); //<3>
        gluTessCallback(_pPimpl->pTesslation, GLU_TESS_ERROR,
            (void (__stdcall *)(void))CustomTessError); //<4>

        return true;
    }

    void Opengl2dPainter::resize(INT w, INT h)
    {
        if (NULL == _pPimpl->hWnd)
            return;

        if(h == 0.0f)
            h = 1.0f;

        _pPimpl->width = w;
        _pPimpl->height = h;

        OpenglContext oglcontext(_pPimpl->hdc, _pPimpl->hrc);

        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho( -_pPimpl->width/2.0f/_pPimpl->scale + _pPimpl->camera.x,  _pPimpl->width/2.0f/_pPimpl->scale + _pPimpl->camera.x,
                 -_pPimpl->height/2.0f/_pPimpl->scale + _pPimpl->camera.y, _pPimpl->height/2.0f/_pPimpl->scale + _pPimpl->camera.y,
                 -1, 1);

        gluLookAt(0, 0, 1,
                0, 0, 0,
                0, 1, 0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void Opengl2dPainter::scale(FLOAT factor)
    {
        _pPimpl->scale = factor;

        OpenglContext oglcontext(_pPimpl->hdc, _pPimpl->hrc);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho( -_pPimpl->width/2.0f/_pPimpl->scale + _pPimpl->camera.x,  _pPimpl->width/2.0f/_pPimpl->scale + _pPimpl->camera.x,
                 -_pPimpl->height/2.0f/_pPimpl->scale + _pPimpl->camera.y, _pPimpl->height/2.0f/_pPimpl->scale + _pPimpl->camera.y,
                 -1, 1);
    }

    void  Opengl2dPainter::rotate(FLOAT angle)
    {
        _pPimpl->angle = angle;
    }

    POINT2F& Opengl2dPainter::camera()
    {
        return _pPimpl->camera;
    }

    void  Opengl2dPainter::viewport(INT dx, INT dy)
    {
        _pPimpl->camera.x -= (float)dx / _pPimpl->scale;
        _pPimpl->camera.y -= (float)dy / _pPimpl->scale;

        OpenglContext oglcontext(_pPimpl->hdc, _pPimpl->hrc);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho( -_pPimpl->width/2.0f/_pPimpl->scale + _pPimpl->camera.x,  _pPimpl->width/2.0f/_pPimpl->scale + _pPimpl->camera.x,
                 -_pPimpl->height/2.0f/_pPimpl->scale + _pPimpl->camera.y, _pPimpl->height/2.0f/_pPimpl->scale + _pPimpl->camera.y,
                 -1, 1);
    }

    POINT2F	Opengl2dPainter::dp2lp(POINT p)
    {
        return this->dp2lp(p.x, p.y);
    }

    POINT2F	Opengl2dPainter::dp2lp(INT x, INT y)
    {
        POINT2F p;

        p.x =  (x - _pPimpl->width/2) / _pPimpl->scale  + _pPimpl->camera.x;
        p.y = -(y - _pPimpl->height/2) / _pPimpl->scale  + _pPimpl->camera.y;

        Vector3 v1(p.x, p.y, 1.0f);
        Matrix3 m_inverse = _pPimpl->worldMatrix.Inverse();
        Vector3 v2 = m_inverse * v1;

        p.x = v2.x;
        p.y = v2.y;
        return p;
    }

    void  Opengl2dPainter::renderBegin(Pen& pen, Matrix3& matrix)
    {
        OpenglContext oglcontext(_pPimpl->hdc, _pPimpl->hrc);

        glClearColor(pen.color().r/ 255.0, pen.color().g/255.0, pen.color().b/255.0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        Matrix3 m;
        m.Rotate( _pPimpl->angle);
        _pPimpl->worldMatrix = matrix * m;
    }

    void  Opengl2dPainter::renderEnd()
    {
        glEnd();

        ::SwapBuffers(_pPimpl->hdc);

        GLenum erreum = glGetError();
        if (GL_NO_ERROR != erreum)
        {
            const GLubyte* message = gluErrorString(erreum);
            /// #if definded(DEBUG) || defined(_DEBUG)
            /// assert(GL_NO_ERROR == erreum);
            /// #endif
        }

        std::set<int>::iterator it;
        it = _pPimpl->displaylists.begin();
        for ( ; it != _pPimpl->displaylists.end(); it++)
        {
            glDeleteLists(*it, 1);
        }

        _pPimpl->displaylists.clear();
    }

    void  Opengl2dPainter::drawBegin(Pen& pen, Matrix3& matrix)
    {
        OpenglContext oglcontext(_pPimpl->hdc, _pPimpl->hrc);

        if (pen.color().a < 255)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        }

        glColor4ub( pen.color().r, pen.color().g, pen.color().b, pen.color().a );
        _pPimpl->pen = pen;
        _pPimpl->localMatrix = matrix;

        glLineWidth( _pPimpl->pen.width());
        if (pen.linestyle() != 0xFFFF)
        {
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, pen.linestyle());
        }

        Matrix3 selectedMovingMatrix;
        selectedMovingMatrix.Identity();

         // apply matrix
        Matrix3 m3 = _pPimpl->worldMatrix * selectedMovingMatrix * _pPimpl->localMatrix;

        FLOAT m[4*4] = {0, };
        m[0] = m3.element[0];   m[1] = m3.element[3];   m[2] = 0.0f;	m[3] = 0.0f;
        m[4] = m3.element[1];   m[5] = m3.element[4];   m[6] = 0.0f;    m[7] = 0.0f;
        m[8] = 0.0f;			m[9] = 0.0f;    		m[10] = 1.0f;	m[11] = 0.0f;
        m[12] = m3.element[2]; 	m[13] = m3.element[5];	m[14] = 0.0f;	m[15] = 1.0f;

        glLoadMatrixf(m);
    }


    void  Opengl2dPainter::drawEnd()
    {
        if (_pPimpl->pen.linestyle() != 0xFFFF)
        {
            glLineStipple(1, 0xFFFF);
            glDisable(GL_LINE_STIPPLE);
        }

        if ( _pPimpl->pen.color().a > 0)
        {
            glDisable(GL_BLEND);
        }
    }


    int	Opengl2dPainter::listBegin(Pen& pen)
    {
        OpenglContext oglcontext(_pPimpl->hdc, _pPimpl->hrc);

        glColor4ub( pen.color().r, pen.color().g, pen.color().b, pen.color().a );
        _pPimpl->pen = pen;

        glLineWidth( _pPimpl->pen.width());
        if (pen.linestyle() != 0xFFFF)
        {
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, pen.linestyle());
        }

        glLoadIdentity();

        int listid = glGenLists(1);
        _pPimpl->displaylists.insert( listid);
        glNewList(listid, GL_COMPILE);

       return listid;
    }

    void Opengl2dPainter::listEnd()
    {
        glEndList();
    }

    void	Opengl2dPainter::drawList(int index)
    {
        std::set<int>::iterator it;
        it = _pPimpl->displaylists.find( index);
        if (it !=  _pPimpl->displaylists.end())
            glCallList(index);
        else
            assert(!"can't find the target display list ");
    }

    void	Opengl2dPainter::drawPoint(POINT2F& point)
    {
        this->drawPoint(point.x , point.y);

    }

    void Opengl2dPainter::drawPoint(FLOAT x, FLOAT y)
    {
        glPointSize( _pPimpl->pen.width() );

        glBegin(GL_POINTS);
            glVertex3f( x, y, 0);
        glEnd();
    }

    void	Opengl2dPainter::drawLine(POINT2F& p1, POINT2F& p2)
    {
        this->drawLine(p1.x, p1.y, p2.x, p2.y);
    }

    void Opengl2dPainter::drawLine(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2)
    {
        glBegin(GL_LINES);
            glVertex3f( x1, y1, 0);
            glVertex3f( x2, y2, 0);
        glEnd();
    }

    void Opengl2dPainter::drawTriangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3, BOOL bFill)
    {
        if (bFill)
        {
            glBegin(GL_TRIANGLES);
                glColor4ub( _pPimpl->pen.color().r, _pPimpl->pen.color().g, _pPimpl->pen.color().b, _pPimpl->pen.color().a );
                glVertex3f( x1, y1, 0);
                glVertex3f( x2, y2, 0);
                glVertex3f( x3, y3, 0);
            glEnd();
        }
        else
        {
            //glRectf
            glBegin(GL_LINE_LOOP);
                glColor4ub( _pPimpl->pen.color().r, _pPimpl->pen.color().g, _pPimpl->pen.color().b, _pPimpl->pen.color().a );
                glVertex3f( x1, y1, 0);
                glVertex3f( x2, y2, 0);
                glVertex3f( x3, y3, 0);
                glVertex3f( x1, y1, 0);
            glEnd();
        }
    }

    void    Opengl2dPainter::drawRectangle(RECT2F& rect, BOOL bFill)
    {
        this->drawRectangle(rect.leftTop.x, rect.leftTop.y, rect.rightBottom.x, rect. rightBottom.y, bFill);
    }

    void Opengl2dPainter::drawRectangle(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, BOOL bFill)
    {
        if (bFill)
        {
            glBegin(GL_QUADS);
                glColor4ub( _pPimpl->pen.color().r, _pPimpl->pen.color().g, _pPimpl->pen.color().b, _pPimpl->pen.color().a ); 	glVertex3f( x1, y1, 0);
                glColor4ub( _pPimpl->pen.color().r, _pPimpl->pen.color().g, _pPimpl->pen.color().b, _pPimpl->pen.color().a ); 	glVertex3f( x2, y1, 0);
                glColor4ub( _pPimpl->pen.color().r, _pPimpl->pen.color().g, _pPimpl->pen.color().b, _pPimpl->pen.color().a ); 	glVertex3f( x2, y2, 0);
                glColor4ub( _pPimpl->pen.color().r, _pPimpl->pen.color().g, _pPimpl->pen.color().b, _pPimpl->pen.color().a ); 	glVertex3f( x1, y2, 0);
            glEnd();

        }
        else
        {
            //glRectf
            glBegin(GL_LINE_LOOP);
                glColor4ub( _pPimpl->pen.color().r, _pPimpl->pen.color().g, _pPimpl->pen.color().b, _pPimpl->pen.color().a );
                glVertex3f( x1, y1, 0);
                glVertex3f( x2, y1, 0);
                glVertex3f( x2, y2, 0);
                glVertex3f( x1, y2, 0);
                glVertex3f( x1, y1, 0);
            glEnd();
        }

    }


    void	Opengl2dPainter::drawArc(POINT2F& ptCenter, FLOAT fRadius, FLOAT fStartAngle, FLOAT fEndAngle)
    {
        this->drawArc( ptCenter.x, ptCenter.y, fRadius, fStartAngle, fEndAngle);
    }

    void Opengl2dPainter::drawArc(FLOAT cx, FLOAT cy, FLOAT fRadius, FLOAT fStartAngle, FLOAT fEndAngle)
    {
    	// positive : ccw

		if (fEndAngle < fStartAngle)
        	fEndAngle += 360.0f;

        if (fStartAngle >= fEndAngle)
        	return;

        glBegin(GL_LINE_STRIP);

            FLOAT fDelta = (fEndAngle - fStartAngle) / 40.0;
            FLOAT fAngle = fStartAngle;

            FLOAT x = fRadius * cos( D2R(fAngle));
            FLOAT y = fRadius * sin( D2R(fAngle));
            glVertex3f( x+ cx, y+cy, 0);
            do
            {
                fAngle += fDelta;
                x = fRadius * cos( D2R(fAngle));
                y = fRadius * sin( D2R(fAngle));
                glVertex3f( x+ cx, y+cy, 0);
            }
            while( fAngle < fEndAngle);

        glEnd();
    }

    void	Opengl2dPainter::drawCircle(POINT2F& ptCenter, FLOAT fRadius, BOOL bFill)
    {
        this->drawCircle(ptCenter.x, ptCenter.y, fRadius, bFill);
    }

    void Opengl2dPainter::drawCircle(FLOAT cx, FLOAT cy, FLOAT fRadius, BOOL bFill)
    {
        if (bFill)
        {
            float y1=cy;
            float x1=cx;

            glBegin(GL_TRIANGLES);
            for(int i=0;i<=360;i+=9 )
            {
                float angle=(float)(((float)i)/57.29577957795135);
                float x2=cx+(fRadius*(float)cos((float)angle));
                float y2=cy+(fRadius*(float)sin((float)angle));

                glColor4ub( _pPimpl->pen.color().r, _pPimpl->pen.color().g, _pPimpl->pen.color().b, _pPimpl->pen.color().a );
                glVertex3d(cx,cy, 0);
                glVertex3d(x1,y1, 0);
                glVertex3d(x2,y2, 0);
                y1=y2;
                x1=x2;
            }
            glEnd();


        }
        else
            this->drawArc(cx, cy, fRadius, 0.0, 360.0);

    }


    void  Opengl2dPainter::drawEllipse(POINT2F& ptCenter, FLOAT majorx, FLOAT minory, FLOAT fStartAngle, FLOAT fEndAngle)
    {
        this->drawEllipse(ptCenter.x, ptCenter.y, majorx,  minory, fStartAngle,  fEndAngle);
    }

    void  Opengl2dPainter::drawEllipse(FLOAT cx, FLOAT cy, FLOAT majorx, FLOAT minory, FLOAT fStartAngle, FLOAT fEndAngle)
    {
        glBegin(GL_LINE_STRIP);

            if (fEndAngle > fStartAngle)
            {
                FLOAT fDelta = (fEndAngle - fStartAngle) / 40.0;
                FLOAT fAngle = fStartAngle;

                FLOAT x = majorx * cos( D2R(fAngle));
                FLOAT y = minory * sin( D2R(fAngle));
                glVertex3f( x+ cx, y+cy, 0);
                do
                {
                    fAngle += fDelta;
                    x = majorx * cos( D2R(fAngle));
                    y = minory * sin( D2R(fAngle));
                    glVertex3f( x+ cx, y+cy, 0);
                }
                while( fAngle < fEndAngle);
            }
            else
            {
                FLOAT fDelta = ( fStartAngle - fEndAngle ) / 40.0;
                FLOAT fAngle = fEndAngle;

                FLOAT x = majorx * cos( D2R(fAngle));
                FLOAT y = minory * sin( D2R(fAngle));
                glVertex3f( x+ cx, y+cy, 0);
                do
                {
                    fAngle += fDelta;
                    x = majorx * cos( D2R(fAngle));
                    y = minory * sin( D2R(fAngle));
                    glVertex3f( x+ cx, y+cy, 0);
                }
                while( fAngle < fStartAngle);
            }

        glEnd();

    }

    void Opengl2dPainter::drawRoundedRect(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT radius)
    {
        //top
        this->drawLine(x1 + radius, y1, x2 - radius, y1);
        //bottom
        this->drawLine(x1 + radius, y2, x2 - radius, y2);
        //left
        this->drawLine(x1, y1 - radius, x1, y2 + radius);
        //right
        this->drawLine(x2, y1 - radius, x2, y2 + radius);

        // arc corner left top
        this->drawArc(x1 + radius, y1 - radius, radius, 90.0f, 180.0f);
        // arc corner right top
        this->drawArc(x2 - radius, y1 - radius, radius, 0.0f, 90.0f);
        // arc corner right bottom
        this->drawArc(x2 - radius, y2 + radius, radius, 270.0f, 360.0f);
        // arc corner left bottom
        this->drawArc(x1 + radius, y2 + radius, radius, 180.0f, 270.0f);
    }

    void Opengl2dPainter::drawRoundedRect(RECT2F& rect, FLOAT radius)
    {
        this->drawRoundedRect(rect.leftTop.x, rect.leftTop.y, rect.rightBottom.x, rect.rightBottom.y, radius);
    }


    #define CONIC_B1(t) (1.0-t)*(1.0-t)
    #define CONIC_B2(t) 2.0*t*(1.0-t)
    #define CONIC_B3(t) (t*t)

    #define CUBIC_B1(t) (1.0-t)*(1.0-t)*(1.0-t)
    #define CUBIC_B2(t) 3.0*t*(1.0-t)*(1.0-t)
    #define CUBIC_B3(t) 3.0*t*t*(1.0-t)
    #define CUBIC_B4(t) t*t*t

    void  Opengl2dPainter::drawBezier(POINT2F* pPoints, INT count, INT steps)
    {
        assert (count == 3 || count ==4 );

        double t = 0.0;
        double x,y;
        double detailBias = 1.0 / (double)steps;

        if (count == 3)
        {
            glBegin(GL_LINE_STRIP);
            do
            {
                x = pPoints[0].x * CONIC_B1(t) + pPoints[1].x * CONIC_B2(t) + pPoints[2].x * CONIC_B3(t);
                y = pPoints[0].y * CONIC_B1(t) + pPoints[1].y * CONIC_B2(t) + pPoints[2].y * CONIC_B3(t);

                glVertex3f( x, y, 0);
                t += detailBias;
            }
            while( t < 1.0);
            glVertex3f( pPoints[2].x, pPoints[2].y, 0);
            glEnd();
        }

        if (count == 4)
        {
            glBegin(GL_LINE_STRIP);
            do
            {
                x = pPoints[0].x * CUBIC_B1(t) + pPoints[1].x * CUBIC_B2(t) + pPoints[2].x * CUBIC_B3(t) + pPoints[3].x * CUBIC_B4(t);
                y = pPoints[0].y * CUBIC_B1(t) + pPoints[1].y * CUBIC_B2(t) + pPoints[2].y * CUBIC_B3(t) + pPoints[3].y * CUBIC_B4(t);

                glVertex3f( x, y, 0);
                t += detailBias;
            }
            while( t < 1.0);
            glVertex3f( pPoints[3].x, pPoints[3].y, 0);
            glEnd();
        }
    }


    typedef struct tagVertexEdge
    {
        double x;
        double y;
        double z;
    }VERTEX_EDGE;

    void  Opengl2dPainter::drawPolygon(const POINT2F* pPoints, INT count, bool bFill)
    {
        /// opengl tesselation

        if (bFill)
            gluTessProperty(_pPimpl->pTesslation, GLU_TESS_BOUNDARY_ONLY, false);
        else
            gluTessProperty(_pPimpl->pTesslation, GLU_TESS_BOUNDARY_ONLY, true);

        VERTEX_EDGE v;
        std::vector<VERTEX_EDGE> vertices;
        vertices.reserve(count+1);

        gluTessBeginPolygon(_pPimpl->pTesslation, NULL);
        gluTessBeginContour(_pPimpl->pTesslation);
            gluNextContour(_pPimpl->pTesslation, GLU_EXTERIOR);

            for (int i=0; i< count; ++i)
            {
                v.x = pPoints[i].x;
                v.y = pPoints[i].y;
                v.z = 0;
                vertices.push_back(v);
                gluTessVertex(_pPimpl->pTesslation, (double*)&vertices[i], (void*) &vertices[i]);
            }

                v.x = pPoints[0].x;
                v.y = pPoints[0].y;
                v.z = 0;
                vertices.push_back(v);

            gluTessVertex(_pPimpl->pTesslation, (double*)&vertices[count],(void*) &vertices[count]);


        gluTessEndContour(_pPimpl->pTesslation);
        gluTessEndPolygon(_pPimpl->pTesslation);

    }



}

