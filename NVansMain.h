// ---------------------------------------------------------------------------

#ifndef NVansMainH
#define NVansMainH

// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.AppEvnts.hpp>

#include "NVansTSettings.h"
#include <Vcl.ExtCtrls.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>

// ---------------------------------------------------------------------------
class TMain : public TForm {
__published:
	TStatusBar *StatusBar;
	TStringGrid *sgServer;
	TStringGrid *sgLocal;
	TApplicationEvents *ApplicationEvents;
	TPanel *PanelServer;
	TButton *btnClose;
	TSplitter *Splitter;
	TLabeledEdit *eRWNum;
	TButton *btnServerLoad;
	TADOConnection *ADOConnection1;
	TADOQuery *ADOQuery1;
	TButton *btnOptions;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ApplicationEventsException(TObject *Sender, Exception *E);
	void __fastcall sgServerDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State);
	void __fastcall sgLocalDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State);
	void __fastcall eRWNumKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift);
	void __fastcall btnServerLoadClick(TObject *Sender);
	void __fastcall sgServerFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall btnOptionsClick(TObject *Sender);

private:
	TSettings * FSettings;

	void CreateServerColumns();
	void CreateLocalColumns();

public:
	__fastcall TMain(TComponent* Owner);

	int DefaultRowHeight;

	void SetControlsEnabled(const bool Enabled);

	__property TSettings * Settings = {read = FSettings};
};

// ---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
// ---------------------------------------------------------------------------
#endif
