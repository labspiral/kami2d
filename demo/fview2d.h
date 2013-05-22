//---------------------------------------------------------------------------

#ifndef fView2DH
#define fView2DH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------

#include "..\..\dll\kamilib.h"
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ActnCtrls.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <ActnMenus.hpp>
#include <ExtActns.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <StdStyleActnCtrls.hpp>
#include <BandActn.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <DockTabSet.hpp>
#include <Tabs.hpp>



class TfrmView2D : public TForm
{
__published:	// IDE-managed Components
	TStatusBar *StatusBar1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TImageList *ImageList1;
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
          TPoint &MousePos, bool &Handled);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClick(TObject *Sender);
	void __fastcall FormDblClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);



public:
	void		initialize();
	void		popUpProperties(TCustomControl* pControl);
    bool		popUpOpenFileDlg();
    bool		popUpSaveFileDlg();
    bool		popUpSerializeDlg();
    bool		saveAgain();
    void		undo();
	void		redo();
	void		remove();
	void		cut();
	void		paste();
	void		copy();
	View2d* 	view2d();

#if (__BORLANDC__ > 0x0630)
	void __fastcall OnGestureEvent(System::TObject* Sender, const TGestureEventInfo &EventInfo, bool &Handled);
    void __fastcall OnKeyPressEvent(TObject *Sender, System::WideChar &Key);
#else
    void __fastcall OnKeyPressEvent(TObject *Sender, char &Key);
#endif

protected:
	View2d*		_pView2d;
	Painter*	_pPainter;
    Selection	_selection;
    String		_sFileName;

public:		// User declarations
	__fastcall TfrmView2D(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmView2D *frmView2D;
extern int  g_viewerflag;

#endif

