//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmain.h"
#include <stdio.h>
#include <tchar.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
	_pView = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	_pView = new TestView;

    _pView->initialize(this->Handle);

    /// touch gesture enable
	this->Touch->InteractiveGestures << TInteractiveGesture::igPan;
	this->Touch->InteractiveGestures << TInteractiveGesture::igZoom;
	this->Touch->InteractiveGestures << TInteractiveGesture::igRotate;
    this->OnGesture = this->OnGestureEvent;

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
	delete _pView;
    _pView = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
	if (_pView)
		_pView->onResize(this->ClientWidth, this->ClientHeight);

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormPaint(TObject *Sender)
{
	DWORD dwEllapsed = _pView->onRender();
    TCHAR str[16];
    _stprintf(str, _T("%d msec"), dwEllapsed);
    StatusBar1->Panels->operator [](2)->Text = str;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{

	switch (Button)
    {
     case mbLeft:
     	_ldown = true;
        _pView->onPanBegin(X, Y);
        break;
     default:
     	break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
	if (_ldown)
		_pView->onPanning(X, Y);


    POINT2F p = _pView->onMouseMove(X, Y);

    TCHAR strX[16], strY[16];
    _stprintf(strX, _T("X %+6.3f"), p.x);
    _stprintf(strY, _T("Y %+6.3f"), p.y);
    StatusBar1->Panels->operator [](0)->Text = strX;
    StatusBar1->Panels->operator [](1)->Text = strY;

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	switch (Button)
    {
     case mbLeft:
     	_ldown = false;
        _pView->onPanEnd(X, Y);
        break;
     default:
     	break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::OnGestureEvent(System::TObject* Sender, const TGestureEventInfo &EventInfo, bool &Handled)
{
    switch(EventInfo.GestureID)
    {
    case igiPan:
        /// use mouse event instead pan gesture
        Handled = true;
    	break;

    case igiZoom:
    	{
            Handled = true;
            if (EventInfo.Flags.Contains(gfBegin))
            {
                _pView->onZoomBegin(  EventInfo.Location.X, EventInfo.Location.Y, EventInfo.Distance);
            }
            else if (EventInfo.Flags.Contains(gfEnd))
            {
                _pView->onZoomEnd(  EventInfo.Location.X, EventInfo.Location.Y, EventInfo.Distance);
            }
            else
            {
                _pView->onZooming(  EventInfo.Location.X, EventInfo.Location.Y, EventInfo.Distance);
            }
        }
        break;

    case igiRotate:
		{
            Handled = true;
            FLOAT angle = R2D(EventInfo.Angle);

            if (EventInfo.Flags.Contains(gfBegin))
            {
                _pView->onRotateBegin(  EventInfo.Location.X, EventInfo.Location.Y, angle);
            }
            else if (EventInfo.Flags.Contains(gfEnd))
            {
                _pView->onRotateEnd(  EventInfo.Location.X, EventInfo.Location.Y, angle);
            }
            else
            {
                _pView->onRotating(  EventInfo.Location.X, EventInfo.Location.Y, angle);
            }
        }
        break;

    default:
        break;
    }
}

void __fastcall TfrmMain::FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
          TPoint &MousePos, bool &Handled)
{
	Handled = true;
	_pView->onMouseWheel(MousePos.X, MousePos.Y, WheelDelta);

}
//---------------------------------------------------------------------------

