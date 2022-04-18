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
#include <Vcl.ExtCtrls.hpp>

#include "NVansTSettings.h"
#include "NVansTOracleVan.h"

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
	void __fastcall FormResize(TObject *Sender);

private:
	TSettings * FSettings;

	TOracleVanList * FServerVanList;

	// -----------------------------------------------------------------------
	void CreateServerColumns();
	void CreateLocalColumns();

	int SetServerVan(int Index, TOracleVan * Van);

	void UpdateServerTrain();

	bool LoadTrain(String TrainNum);

public:
	__fastcall TMain(TComponent* Owner);

	int DefaultRowHeight;

	void SetControlsEnabled(const bool Enabled);

	// -----------------------------------------------------------------------
	__property TSettings * Settings = {read = FSettings};

	__property TOracleVanList * ServerVanList = {read = FServerVanList};
};

// ---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
// ---------------------------------------------------------------------------
#endif
