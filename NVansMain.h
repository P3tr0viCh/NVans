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
#include <Vcl.Dialogs.hpp>

#include <IntegerPair.h>

#include <DBOperationEvent.h>

#include "NVansColumns.h"

#include "NVansSettings.h"

#include "NVansLocalVan.h"
#include "NVansOracleVan.h"

#include "NVansKeyOracleTrain.h"

// ---------------------------------------------------------------------------
class TMain : public TForm, public IDBOperationEvent {
__published:
	TStatusBar *StatusBar;
	TStringGrid *sgServer;
	TStringGrid *sgLocal;
	TApplicationEvents *ApplicationEvents;
	TPanel *PanelServer;
	TButton *btnClose;
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
	TButton *btnAbout;
	TPopupMenu *PopupMenu;
	TMenuItem *miSelectAll;
	TMenuItem *miCopy;
	TMenuItem *miPasteVanNum;
	TMenuItem *miClear;
	TButton *btnServerReverse;
	TLabel *lblRWNum;
	TEdit *eRWNum;
	TLabel *lblDateTime;
	TEdit *eDateTime;
	TButton *btnServerSaveToFile;
	TSaveDialog *SaveDialog;
	TMenuItem *miVanToString;
	TButton *btnServerLoadExt;
	TPopupMenu *pmServerLoadExt;
	TMenuItem *miServerLoadAsIs;

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
	void __fastcall btnLocalLoadClick(TObject *Sender);
	void __fastcall btnSaveVanPropsClick(TObject *Sender);
	void __fastcall btnCopyDataAllClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnLocalSaveClick(TObject *Sender);
	void __fastcall sgServerSelectCell(TObject *Sender, int ACol, int ARow,
		bool &CanSelect);
	void __fastcall TimerResizeTimer(TObject *Sender);
	void __fastcall sgLocalDblClick(TObject *Sender);
	void __fastcall btnLocalTrainsClick(TObject *Sender);
	void __fastcall btnAboutClick(TObject *Sender);
	void __fastcall miCopyClick(TObject *Sender);
	void __fastcall miSelectAllClick(TObject *Sender);
	void __fastcall miPasteVanNumClick(TObject *Sender);
	void __fastcall PopupMenuPopup(TObject *Sender);
	void __fastcall miClearClick(TObject *Sender);
	void __fastcall btnServerReverseClick(TObject *Sender);
	void __fastcall eRWNumChange(TObject *Sender);
	void __fastcall btnServerSaveToFileClick(TObject *Sender);
	void __fastcall sgServerKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift);
	void __fastcall sgServerMouseUp(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y);
	void __fastcall sgServerMouseDown(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y);
	void __fastcall sgLocalSelectCell(TObject *Sender, int ACol, int ARow,
		bool &CanSelect);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall miVanToStringClick(TObject *Sender);
	void __fastcall miServerLoadAsIsClick(TObject *Sender);
	void __fastcall btnServerLoadExtClick(TObject *Sender);

private:
	TSettings * FSettings;

	TServerColumns * ServerColumns;
	TLocalColumns * LocalColumns;

	TStringGridOptions * ServerOptions;
	TStringGridOptions * LocalOptions;

	TKeyOracleTrain * FKeyOracleTrain;
	String FLocalTrainNum;

	TDate FDateLocal;

	bool FLocalChanged;

	bool FUseAutoReplace;

	TOracleVanList * FServerVanList;
	TLocalVanList * FLocalVanList;

	int ServerSelectedRow;
	int LocalSelectedRow;

	// -----------------------------------------------------------------------
	void SetControlsEnabled(const bool Enabled);

	void UpdateScaleType();

	void UpdateStatusBar();

	void SettingsChanged();

	int SetServerVan(int Index, TOracleVan * Van);
	int SetLocalVan(int Index, TLocalVan * Van);

	void SetLocalChanged(bool Changed);

	void SetKeyOracleTrain(TKeyOracleTrain * Value);
	void SetLocalTrainNum(String Value);

	void SetDateLocal(TDate Value);

	void SetServerVanList(TOracleVanList * Value);
	void SetLocalVanList(TLocalVanList * Value);

	void ServerLoadTrain();
	void ServerLoadTrainDateTime(TKeyOracleTrain * KeyOracleTrain);

	void ServerLoadTrainPerform();

	bool ServerSaveTrainToFile(TOracleVanList * ServerVanList, String FileName);

	void AutoReplace(TOracleVanList * ServerVanList);

	void LocalLoadVans();
	bool LocalSaveVans();
	void LocalSaveVanProps();

	bool CheckField(int Column1, int Column2, int Index1, int Index2);
	bool DataExists(TIntegerPairList * Result);

	void CopyData(bool CopyAll);
	void SendDataToWME();

	enum TAvitekBtn {
		abSave, abUpdate
	};

	HWND AvitekGetProtFormBtn(TAvitekBtn AvitekBtn);
	bool AvitekCheckNeedSave();
	void AvitekUpdateProt();

	enum TMenuItemAction {
		maCopy, maPaste, maClear, maSelectAll, maVanToString
	};

	void MenuItemAction(TMenuItemAction Action);

	void DBOperationEventStart(TObject * Sender);
	void DBOperationEventEndOK(TObject * Sender);
	void DBOperationEventEndFail(TObject * Sender);

public:
	__fastcall TMain(TComponent* Owner);

	enum TOperation {
		oLoad, oSave, oSendToWME
	};

	// -----------------------------------------------------------------------
	void StartOperation(TOperation Operation);

	void EndOperation();

	void KeyOracleTrainChanged();

	void __fastcall StartSearchEvent(TObject * Sender, String &Text,
		bool &Found);

	// -----------------------------------------------------------------------
	__property TSettings * Settings = {read = FSettings};

	__property TKeyOracleTrain * KeyOracleTrain = {
		read = FKeyOracleTrain, write = SetKeyOracleTrain};
	__property String LocalTrainNum = {
		read = FLocalTrainNum, write = SetLocalTrainNum};

	__property TDate DateLocal = {read = FDateLocal, write = SetDateLocal};

	__property bool LocalChanged = {
		read = FLocalChanged, write = SetLocalChanged};

	__property bool UseAutoReplace = {
		read = FUseAutoReplace, write = FUseAutoReplace};

	__property TOracleVanList * ServerVanList = {
		read = FServerVanList, write = SetServerVanList};
	__property TLocalVanList * LocalVanList = {
		read = FLocalVanList, write = SetLocalVanList};
};

// ---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
// ---------------------------------------------------------------------------
#endif
