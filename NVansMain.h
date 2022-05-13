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

#include "NVansTVan.h"

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
	TButton *btnServerList;
	TPanel *PanelLocal;
	TButton *btnLocalLoad;
	TPanel *PanelCommon;
	TButton *btnSaveVanProps;
	TButton *btnCopyData;

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
	void __fastcall btnServerListClick(TObject *Sender);
	void __fastcall sgServerKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift);
	void __fastcall btnLocalLoadClick(TObject *Sender);
	void __fastcall btnSaveVanPropsClick(TObject *Sender);
	void __fastcall btnCopyDataClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:
	TSettings * FSettings;

	String FTrainNum;

	TDate FDateLocal;

	TVanList * FServerVanList;
	TVanList * FLocalVanList;

	// -----------------------------------------------------------------------
	void CreateServerColumns();
	void CreateLocalColumns();

	void SetControlsEnabled(const bool Enabled);

	void SetUseLocal();

	int SetServerVan(int Index, TVan * Van);
	int SetLocalVan(int Index, TVan * Van);

	void SetTrainNum(String Value);

	void SetDateLocal(TDate Value);

	void SetServerVanList(TVanList * Value);
	void SetLocalVanList(TVanList * Value);

	bool ServerLoadTrain(String TrainNum, bool WithJoin);
	bool LocalLoadVans();
	bool LocalSaveVanProps();

public:
	__fastcall TMain(TComponent* Owner);

	int DefaultRowHeight;

	// -----------------------------------------------------------------------
	void StartLoad();
	void EndLoad();

	// -----------------------------------------------------------------------
	__property TSettings * Settings = {read = FSettings};

	__property String TrainNum = {read = FTrainNum, write = SetTrainNum};

	__property TDate DateLocal = {read = FDateLocal, write = SetDateLocal};

	__property TVanList * ServerVanList = {
		read = FServerVanList, write = SetServerVanList};
	__property TVanList * LocalVanList = {
		read = FLocalVanList, write = SetLocalVanList};
};

// ---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
// ---------------------------------------------------------------------------
#endif
