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
#include <Vcl.Menus.hpp>

#include <IntegerPair.h>

#include "NVansTSettings.h"

#include "NVansTLocalVan.h"
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
	TButton *btnServerTrains;
	TPanel *PanelLocal;
	TButton *btnLocalLoad;
	TPanel *PanelCommon;
	TButton *btnSaveVanProps;
	TButton *btnCopyDataAll;
	TButton *btnLocalSave;
	TTimer *TimerResize;
	TButton *btnCopyDataMass;
	TButton *btnLocalTrains;

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
	void __fastcall btnServerTrainsClick(TObject *Sender);
	void __fastcall sgServerKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift);
	void __fastcall btnLocalLoadClick(TObject *Sender);
	void __fastcall btnSaveVanPropsClick(TObject *Sender);
	void __fastcall btnCopyDataAllClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnLocalSaveClick(TObject *Sender);
	void __fastcall sgServerSelectCell(TObject *Sender, int ACol, int ARow,
		bool &CanSelect);
	void __fastcall sgLocalSelectCell(TObject *Sender, int ACol, int ARow,
		bool &CanSelect);
	void __fastcall TimerResizeTimer(TObject *Sender);
	void __fastcall sgLocalDblClick(TObject *Sender);
	void __fastcall btnLocalTrainsClick(TObject *Sender);

private:
	TSettings * FSettings;

	String FOracleTrainNum;
	String FLocalTrainNum;

	TDate FDateLocal;

	bool FLocalChanged;

	int ServerSelectedRow;
	int LocalSelectedRow;

	TOracleVanList * FServerVanList;
	TLocalVanList * FLocalVanList;

	// -----------------------------------------------------------------------
	void CreateServerColumns();
	void CreateLocalColumns();

	void SetControlsEnabled(const bool Enabled);

	void SetUseLocal();

	int SetServerVan(int Index, TOracleVan * Van);
	int SetLocalVan(int Index, TLocalVan * Van);

	void SetLocalVanChanged(int Index, bool Changed);
	bool IsLocalVanChanged(int Index);

	void SetLocalVanTare(int Index, bool Tare);
	bool IsLocalVanTare(int Index);

	void SetLocalChanged(bool Changed);

	TOracleVan * GetServerVan(int Index);
	TLocalVan * GetLocalVan(int Index);

	void SetOracleTrainNum(String Value);
	void SetLocalTrainNum(String Value);

	void SetDateLocal(TDate Value);

	void SetServerVanList(TOracleVanList * Value);
	void SetLocalVanList(TLocalVanList * Value);

	bool ServerLoadTrain(String TrainNum, bool WithJoin);
	bool LocalLoadVans();
	bool LocalSaveVanProps();
	bool LocalSaveVans();

	bool CheckField(int Column1, int Column2, int Index1, int Index2);
	bool DataExists(TIntegerPairList * Result);
	void CopyData(bool CopyAll);

	enum TAvitekBtn {
		abSave, abUpdate
	};

	HWND AvitekGetProtFormBtn(TAvitekBtn AvitekBtn);
	bool AvitekCheckNeedSave();
	void AvitekUpdateProt();

public:
	__fastcall TMain(TComponent* Owner);

	int DefaultRowHeight;

	enum TDBOperation {
		dboLoad, dboSave
	};

	// -----------------------------------------------------------------------
	void StartDBOperation(TDBOperation DBOperation);

	void EndDBOperation();

	// -----------------------------------------------------------------------
	__property TSettings * Settings = {read = FSettings};

	__property String OracleTrainNum = {
		read = FOracleTrainNum, write = SetOracleTrainNum};
	__property String LocalTrainNum = {
		read = FLocalTrainNum, write = SetLocalTrainNum};

	__property TDate DateLocal = {read = FDateLocal, write = SetDateLocal};

	__property bool LocalChanged = {
		read = FLocalChanged, write = SetLocalChanged};

	__property TOracleVanList * ServerVanList = {
		read = FServerVanList, write = SetServerVanList};
	__property TLocalVanList * LocalVanList = {
		read = FLocalVanList, write = SetLocalVanList};
};

// ---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
// ---------------------------------------------------------------------------
#endif
