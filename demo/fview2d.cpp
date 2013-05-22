#include "fview2d.h"
#include <stdio.h>
#include <tchar.h>
#include "..\..\viewer\view2ddrilling.h"
#include "..\..\viewer\view2ddxf.h"
#include "..\..\viewer\view2difont.h"
#include "..\..\viewer\view2difonteditor.h"
#include "..\..\viewer\view2dsubstrate.h"
#include "..\..\viewer\view2dtemperedglass.h"
#include "..\..\viewer\view2dunit.h"
#include "..\..\viewer\view2dwafer.h"
#include "..\..\viewer\view2dimage.h"
#include "..\..\viewer\view2dfreetype.h"
#include "..\..\viewer\view2dcorrection.h"
#include "..\..\viewer\view2dledassembly.h"

#pragma comment (lib, "gdiplus.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmView2D *frmView2D;
//---------------------------------------------------------------------------

int g_viewerflag = 1;


__fastcall TfrmView2D::TfrmView2D(TComponent* Owner)
	: TForm(Owner)
{
	_pView2d = NULL;
    _pPainter = NULL;
}
//---------------------------------------------------------------------------

void TfrmView2D::initialize()
{
	_pView2d->initialize(this->Handle);

    //for safety reason
    _pView2d->resize(this->ClientWidth, this->ClientHeight);
}
//---------------------------------------------------------------------------
void __fastcall TfrmView2D::FormCreate(TObject *Sender)
{
	assert(NULL == _pPainter);
   	_pPainter = new Opengl2DPainter(&_selection);
    assert(_pPainter);


    assert(NULL == _pView2d);
    switch (g_viewerflag)
    {
    case 1:
    	_pView2d = new View2dWafer(_pPainter, &_selection);
        break;
    case 2:
    	_pView2d = new View2dSubstrate(_pPainter, &_selection);
        break;
    case 3:
		_pView2d = new View2dUnit(_pPainter, &_selection);
        break;
    case 4:
    	_pView2d = new View2dDrilling(_pPainter, &_selection);
        break;
    case 5:
    	_pView2d = new View2dTemperedGlass(_pPainter, &_selection);
        break;
    case 6:
    	_pView2d = new View2ddxf(_pPainter, &_selection);
        break;
    case 7:
    	_pView2d = new View2dIFont(_pPainter, &_selection);
        break;
    case 8:
    	_pView2d = new View2dIFontEditor(_pPainter, &_selection);
        break;
    case 9:
    	_pView2d = new View2dImage(_pPainter, &_selection);
        break;
    case 10:
    	_pView2d = new View2dFreetype(_pPainter, &_selection);
        break;
    case 11:
    	_pView2d = new View2dCorrection(_pPainter, &_selection);
        break;
    case 12:
    	_pView2d = new View2dLedAssembly(_pPainter, &_selection);
        break;
    }
    assert(_pView2d);

#if (__BORLANDC__ > 0x0630)
	this->Touch->InteractiveGestures << TInteractiveGesture::igPan;
	this->Touch->InteractiveGestures << TInteractiveGesture::igZoom;
	this->Touch->InteractiveGestures << TInteractiveGesture::igRotate;
	this->OnGesture = this->OnGestureEvent;
#endif
	this->OnKeyPress = this->OnKeyPressEvent;
}
//---------------------------------------------------------------------------

void __fastcall TfrmView2D::FormDestroy(TObject *Sender)
{
	delete _pView2d;
    _pView2d = NULL;

	delete _pPainter;
    _pPainter = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmView2D::FormResize(TObject *Sender)
{
	if (_pView2d)
    {
		_pView2d->resize(this->ClientWidth, this->ClientHeight);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmView2D::FormPaint(TObject *Sender)
{
	DWORD dwEllapsed = _pView2d->render();
    TCHAR str[16];
    _stprintf(str, _T("%d msec"), dwEllapsed);
    StatusBar1->Panels->operator [](2)->Text = str;

}
//---------------------------------------------------------------------------
void __fastcall TfrmView2D::FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
          TPoint &MousePos, bool &Handled)
{
    Handled = true;
	_pView2d->mouseWheel(WheelDelta, MousePos.x, MousePos.y);

    TCHAR str[8];
    _stprintf(str, _T("* %.1f"),_pView2d->painter()->getScale() );
    StatusBar1->Panels->operator [](4)->Text = str;
}
//---------------------------------------------------------------------------

void __fastcall TfrmView2D::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	SetFocus();
	_pView2d->mouseDown( (MOUSE_BUTTON)Button, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmView2D::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	int count = _pView2d->mouseUp( (MOUSE_BUTTON)Button, X, Y);

    TCHAR str[16];
    _stprintf(str, _T("%d selected"), count);
    StatusBar1->Panels->operator [](3)->Text = str;
}
//---------------------------------------------------------------------------

void __fastcall TfrmView2D::FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
	POINT2F p = _pView2d->mouseMove(X, Y);

    TCHAR strX[16], strY[16];
    _stprintf(strX, _T("X %+6.3f"), p.x);
    _stprintf(strY, _T("Y %+6.3f"), p.y);
    StatusBar1->Panels->operator [](0)->Text = strX;
    StatusBar1->Panels->operator [](1)->Text = strY;
}
//---------------------------------------------------------------------------

void __fastcall TfrmView2D::FormClick(TObject *Sender)
{
    _pView2d->click();
}
//---------------------------------------------------------------------------

void __fastcall TfrmView2D::FormDblClick(TObject *Sender)
{
    _pView2d->dblClick();
}
//---------------------------------------------------------------------------
void __fastcall TfrmView2D::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    _pView2d->keyDown(Key);
}
//---------------------------------------------------------------------------

void __fastcall TfrmView2D::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
    _pView2d->keyUp(Key);
}
//---------------------------------------------------------------------------
void TfrmView2D::popUpProperties(TCustomControl* pControl)
{
#if defined(__BORLANDC__)
	_pView2d->popUpProperties(pControl);
#endif    
}

bool TfrmView2D::popUpOpenFileDlg()
{
	OpenDialog1->Title = _T("Open the file ...");
    OpenDialog1->Filter = _pView2d->getFileFilter();

    if (!OpenDialog1->Execute())
    	return false;


    String strFileName = OpenDialog1->FileName;
    if (_pView2d->open(  strFileName.c_str()))
    {
	    _sFileName =  strFileName;
        _pView2d->render();

        StatusBar1->Panels->operator [](6)->Text = _sFileName;
    	return true;
    }
    return false;
}

bool TfrmView2D::popUpSaveFileDlg()
{
	SaveDialog1->Title = _T("Save the file ...");
    SaveDialog1->Filter = _pView2d->getFileFilter();

    if (!SaveDialog1->Execute())
    	return false;

    String strFileName = SaveDialog1->FileName;
    if (_pView2d->save(  strFileName.c_str() ))
	{
	    _sFileName =  strFileName;
        StatusBar1->Panels->operator [](6)->Text = _sFileName;
		return true;
    }
    return false;
}


bool TfrmView2D::popUpSerializeDlg()
{
	SaveDialog1->Title = _T("Serialize the file ...");
    SaveDialog1->Filter = _T("da0 files (*.da0)|*.da0|all files (*.*)|*.*");

    if (!SaveDialog1->Execute())
    	return false;

    String strFileName = SaveDialog1->FileName;
    if (_pView2d->serialize(  strFileName.c_str() ))
	{
		return true;
    }
    return false;
}


bool  TfrmView2D::saveAgain()
{
	if (_sFileName == "")
    	return false;

    if (_pView2d->save( _sFileName.c_str() ))
	{
		return true;
    }
	return false;
}

void TfrmView2D::undo()
{
	_pView2d->undo();
}

void TfrmView2D::redo()
{
	_pView2d->redo();
}

void TfrmView2D::remove()
{
	_pView2d->remove();
}

void TfrmView2D::cut()
{
	//_pView2d->cut();
}

void TfrmView2D::paste()
{
	//_pView2d->paste();
}
void TfrmView2D::copy()
{
	//_pView2d->copy();
}
//---------------------------------------------------------------------------


View2d* TfrmView2D::view2d()
{
	return _pView2d;
}

void __fastcall TfrmView2D::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;
}
//---------------------------------------------------------------------------


#if (__BORLANDC__ > 0x0630)
void __fastcall TfrmView2D::OnGestureEvent(System::TObject* Sender, const TGestureEventInfo &EventInfo, bool &Handled)
{
    switch(EventInfo.GestureID)
    {
    case igiZoom:
    	{
            Handled = true;
            if (EventInfo.Flags.Contains(gfBegin))
            {
                _pView2d->touchZoomBegin(  EventInfo.Location.X, EventInfo.Location.Y, EventInfo.Distance);
            }
            else if (EventInfo.Flags.Contains(gfEnd))
            {
                _pView2d->touchZoomEnd(  EventInfo.Location.X, EventInfo.Location.Y, EventInfo.Distance);
            }
            else
            {
                _pView2d->touchZooming(  EventInfo.Location.X, EventInfo.Location.Y, EventInfo.Distance);
            }
            TCHAR str[8];
            _stprintf(str, _T("* %.3f"), _pView2d->painter()->getScale());
            StatusBar1->Panels->operator [](4)->Text = str;
        }
        break;

    case igiRotate:
		{
            Handled = true;
            FLOAT angle = R2D(EventInfo.Angle);

            if (EventInfo.Flags.Contains(gfBegin))
            {
                _pView2d->touchRotateBegin(  EventInfo.Location.X, EventInfo.Location.Y, angle);
            }
            else if (EventInfo.Flags.Contains(gfEnd))
            {
                _pView2d->touchRotateEnd(  EventInfo.Location.X, EventInfo.Location.Y, angle);
            }
            else
            {
                _pView2d->touchRotating(  EventInfo.Location.X, EventInfo.Location.Y, angle);
            }
    		TCHAR str[8];
            _stprintf(str, _T("%+6.1f¨¬"), _pView2d->painter()->getRotate() );
            StatusBar1->Panels->operator [](5)->Text = str;
        }
        break;

    case igiPressAndTap:
        if (EventInfo.Flags.Contains(gfEnd))
            _pView2d->popUpProperties(NULL);
    	break;

    default:
        break;
    }
}

void __fastcall TfrmView2D::OnKeyPressEvent(TObject *Sender, System::WideChar &Key)
{
	int count = _pView2d->keyPress(Key);

    TCHAR str[16];
    _stprintf(str, _T("%d selected"), count);
    StatusBar1->Panels->operator [](3)->Text = str;
}

#else

void __fastcall TfrmView2D::OnKeyPressEvent(TObject *Sender, char &Key)
{
	int count = _pView2d->keyPress(Key);

    TCHAR str[16];
    _stprintf(str, _T("%d selected"), count);
    StatusBar1->Panels->operator [](3)->Text = str;
}

#endif
//---------------------------------------------------------------------------


