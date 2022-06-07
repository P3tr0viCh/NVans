// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Clipbrd.hpp"

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsKAndM.h>
#include <UtilsFiles.h>
#include <UtilsFileIni.h>
#include <UtilsStringGrid.h>

#include "NVansDebug.h"

#include "NVansAdd.h"
#include "NVansColumns.h"
#include "NVansStrings.h"
#include "NVansStringsGridHeader.h"

#include "NVansFindMatch.h"

#include "NVansTDBOracleLoadTrain.h"
#include "NVansTDBLocalLoadVans.h"
#include "NVansTDBLocalSaveVanProps.h"
#include "NVansTDBLocalSaveVan.h"

#include "NVansLogin.h"
#include "NVansOptions.h"
#include "NVansServerTrains.h"
#include "NVansLocalTrains.h"

#include "NVansMain.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TMain *Main;

static TNVansServerColumns ServerColumns;
static TNVansLocalColumns LocalColumns;

// ---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormCreate(TObject *Sender) {
	WriteToLogProgramStart();

	FSettings = new TSettings();

	FOracleTrainNum = "";

	ServerSelectedRow = -1;
	LocalSelectedRow = -1;

	FServerVanList = new TOracleVanList();
	FLocalVanList = new TLocalVanList();

	DefaultRowHeight = Canvas->TextHeight("ComboBox") + 8;

	Caption = Application->Title + " " + GetFileVer(Application->ExeName);
	StatusBar->SimpleText = LoadStr(IDS_COPYRIGHT_STATUS);

	CreateServerColumns();
	CreateLocalColumns();

	sgServer->DefaultRowHeight = Main->DefaultRowHeight; // >> ComboBox->Height;
	sgLocal->DefaultRowHeight = Main->DefaultRowHeight;

	sgServer->Options = sgServer->Options << goColSizing;
	sgLocal->Options = sgLocal->Options << goColSizing;

	TFileIni * FileIni = TFileIni::GetNewInstance();
	try {
		sgServer->Height = FileIni->ReadInteger(Name, "ServerHeight",
			sgServer->Height);
		Splitter->Top = sgServer->Top + sgServer->Height;
		PanelCommon->Top = Splitter->Top + Splitter->Height;

		FileIni->ReadFormBounds(this);

		StringGridColWidthsReadFromIni(sgServer, FileIni, Name,
			"ServerColWidths");
		StringGridColWidthsReadFromIni(sgLocal, FileIni, Name,
			"LocalColWidths");
	}
	__finally {
		delete FileIni;
	}

#ifdef _DEBUG
	eRWNum->Text = "42";
#else
#endif

	if (!Settings->Load()) {
		MsgBoxErr(IDS_ERROR_LOAD_SETTINGS);

		Application->Terminate();

		return;
	}

	if (!Settings->UseLocal) {
		SetUseLocal();
	}

#ifdef FIND_MATCH_TEST
	int FindMatchTestResult = FindMatchTest();
	if (FindMatchTestResult) {
		MsgBoxErr("TEST " + IToS_0(FindMatchTestResult) + ": FAIL");
	}
	else {
		MsgBox("ALL TESTS OK");
	}
	Application->Terminate();
	return;
#endif
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormDestroy(TObject *Sender) {
	TFileIni * FileIni = TFileIni::GetNewInstance();
	try {
		FileIni->WriteFormBounds(this);

		FileIni->WriteInteger(Name, "ServerHeight", sgServer->Height);

		StringGridColWidthsWriteToIni(sgServer, FileIni, Name,
			"ServerColWidths");
		StringGridColWidthsWriteToIni(sgLocal, FileIni, Name, "LocalColWidths");
	}
	__finally {
		delete FileIni;
	}

	FLocalVanList->Free();
	FServerVanList->Free();

	FSettings->Free();

	WriteToLogProgramStop();
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnCloseClick(TObject *Sender) {
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormCloseQuery(TObject *Sender, bool &CanClose) {
#ifndef FORCECLOSE
	CanClose = MsgBoxYesNo(LocalChanged ? IDS_QUESTION_CLOSE_PROGRAM_CHANGED :
		IDS_QUESTION_CLOSE_PROGRAM, true, Application->Handle);
#endif
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormShow(TObject *Sender) {
#ifdef FIND_MATCH_TEST
	return;
#endif

#ifdef _DEBUG
	btnServerLoad->Click();
	btnLocalLoad->Click();
#endif
}

// ---------------------------------------------------------------------------
void __fastcall TMain::ApplicationEventsException(TObject *Sender, Exception *E)
{
	WriteToLog(Format(IDS_LOG_ERROR_EXCEPTION, E->Message));
	MsgBoxErr(Format(IDS_ERROR_UNKNOWN_EXCEPTION, E->Message));
}

// ---------------------------------------------------------------------------
void TMain::CreateServerColumns() {
	sgServer->ColCount = ServerColumns.VISIBLE_COUNT;

	StringGridSetHeader(sgServer, ServerColumns.NUM, IDS_GRID_HEADER_NUM, 32);

	StringGridSetHeader(sgServer, ServerColumns.VANNUM,
		IDS_GRID_HEADER_VANNUM, 80);

	StringGridSetHeader(sgServer, ServerColumns.CARGOTYPE,
		IDS_GRID_HEADER_CARGOTYPE, 180);

	StringGridSetHeader(sgServer, ServerColumns.INVOICE_NUM,
		IDS_GRID_HEADER_INVOICE_NUM, 180);
	StringGridSetHeader(sgServer, ServerColumns.INVOICE_SUPPLIER,
		IDS_GRID_HEADER_INVOICE_SUPPLIER, 180);
	StringGridSetHeader(sgServer, ServerColumns.INVOICE_RECIPIENT,
		IDS_GRID_HEADER_INVOICE_RECIPIENT, 180);
	StringGridSetHeader(sgServer, ServerColumns.DEPART_STATION,
		IDS_GRID_HEADER_DEPART_STATION, 180);
	StringGridSetHeader(sgServer, ServerColumns.PURPOSE_STATION,
		IDS_GRID_HEADER_PURPOSE_STATION, 180);
	StringGridSetHeader(sgServer, ServerColumns.CARRYING,
		IDS_GRID_HEADER_CARRYING, 64);
	StringGridSetHeader(sgServer, ServerColumns.TARE_T,
		IDS_GRID_HEADER_TARE_T, 64);
	StringGridSetHeader(sgServer, ServerColumns.INVOICE_NETTO,
		IDS_GRID_HEADER_INVOICE_NETTO, 64);
	StringGridSetHeader(sgServer, ServerColumns.INVOICE_TARE,
		IDS_GRID_HEADER_INVOICE_TARE, 64);
}

// ---------------------------------------------------------------------------
void TMain::CreateLocalColumns() {
	sgLocal->ColCount = LocalColumns.VISIBLE_COUNT;

	StringGridSetHeader(sgLocal, LocalColumns.NUM, IDS_GRID_HEADER_NUM, 32);
	StringGridSetHeader(sgLocal, LocalColumns.DATETIME,
		IDS_GRID_HEADER_DATETIME, 160);

	StringGridSetHeader(sgLocal, LocalColumns.VANNUM,
		IDS_GRID_HEADER_VANNUM, 80);

	StringGridSetHeader(sgLocal, LocalColumns.CARGOTYPE,
		IDS_GRID_HEADER_CARGOTYPE, 180);

	StringGridSetHeader(sgLocal, LocalColumns.INVOICE_NUM,
		IDS_GRID_HEADER_INVOICE_NUM, 180);
	StringGridSetHeader(sgLocal, LocalColumns.INVOICE_SUPPLIER,
		IDS_GRID_HEADER_INVOICE_SUPPLIER, 180);
	StringGridSetHeader(sgLocal, LocalColumns.INVOICE_RECIPIENT,
		IDS_GRID_HEADER_INVOICE_RECIPIENT, 180);
	StringGridSetHeader(sgLocal, LocalColumns.DEPART_STATION,
		IDS_GRID_HEADER_DEPART_STATION, 180);
	StringGridSetHeader(sgLocal, LocalColumns.PURPOSE_STATION,
		IDS_GRID_HEADER_PURPOSE_STATION, 180);

	StringGridSetHeader(sgLocal, LocalColumns.CARRYING,
		IDS_GRID_HEADER_CARRYING, 64);
	StringGridSetHeader(sgLocal, LocalColumns.TARE_T,
		IDS_GRID_HEADER_TARE_T, 64);
	StringGridSetHeader(sgLocal, LocalColumns.INVOICE_NETTO,
		IDS_GRID_HEADER_INVOICE_NETTO, 64);
	StringGridSetHeader(sgLocal, LocalColumns.INVOICE_TARE,
		IDS_GRID_HEADER_INVOICE_TARE, 64);

	StringGridSetHeader(sgLocal, LocalColumns.NETTO_DIFF,
		IDS_GRID_HEADER_NETTO_DIFF, 64);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgServerDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State) {
	StringGridDrawCell(sgServer, ACol, ARow, Rect, State, NUSet,
		ServerColumns.LeftAlign, NUSet, Main->Settings->ColorReadOnly, NUColor,
		true, false, false, NUColor, true, Main->Settings->ColorSelected);
}

const TIntegerSet LocalColumnsRowIsTare = TIntegerSet() << LocalColumns.VANNUM;

// ---------------------------------------------------------------------------
void __fastcall TMain::sgLocalDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State) {
	StringGridDrawCell(sgLocal, ACol, ARow, Rect, State, IsLocalVanTare(ARow) ?
		LocalColumnsRowIsTare : NUSet, LocalColumns.LeftAlign, NUSet,
		Main->Settings->ColorReadOnly, NUColor, true, false,
		IsLocalVanChanged(ARow), Main->Settings->ColorChanged, true,
		Main->Settings->ColorSelected);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::eRWNumKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift) {
	if (Key == VK_RETURN && Shift.Empty()) {
		btnServerLoad->Click();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgServerKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift) {
	TStringGrid * S = (TStringGrid*) Sender;

	if (StringGridIsEmpty(S)) {
		return;
	}

	if (Key == 'C' && Shift == (TShiftState() << ssCtrl)) {
		Clipboard()->AsText = S->Cells[S->Col][S->Row];
	}
}

// ---------------------------------------------------------------------------
void TMain::SetControlsEnabled(const bool Enabled) {
	PanelServer->Enabled = Enabled;
	sgServer->Enabled = Enabled;
	Splitter->Enabled = Enabled;
	PanelCommon->Enabled = Enabled;
	sgLocal->Enabled = Enabled;
	PanelLocal->Enabled = Enabled;
}

// ---------------------------------------------------------------------------
void TMain::SetUseLocal() {
	sgServer->Align = Settings->UseLocal ? alTop : alClient;
	if (Settings->UseLocal) {
		sgServer->Height = ClientHeight / 2 - PanelServer->Height -
			PanelLocal->Height + StatusBar->Height;
		Splitter->Top = sgServer->Top + sgServer->Height;
		PanelCommon->Top = Splitter->Top + Splitter->Height;
	}
	Splitter->Visible = Settings->UseLocal;
	PanelCommon->Visible = Settings->UseLocal;
	sgLocal->Visible = Settings->UseLocal;
	PanelLocal->Visible = Settings->UseLocal;
}

// ---------------------------------------------------------------------------
void TMain::StartDBOperation(TDBOperation DBOperation) {
	ShowWaitCursor();

	NativeUInt Ident = 0;

	switch (DBOperation) {
	case dboLoad:
		Ident = IDS_STATUS_TRAIN_LOAD;
		break;
	case dboSave:
		Ident = IDS_STATUS_TRAIN_SAVE;
		break;
	}

	StatusBar->SimpleText = LoadStr(Ident);

	SetControlsEnabled(false);

	frmServerTrains->StartLoad();
	frmLocalTrains->StartLoad();

	ProcMess();
}

// ---------------------------------------------------------------------------
void TMain::EndDBOperation() {
	frmLocalTrains->EndLoad();
	frmServerTrains->EndLoad();

	SetControlsEnabled(true);

	StatusBar->SimpleText = "";

	btnSaveVanProps->Enabled = !StringGridIsEmpty(sgServer);
	btnCopyDataAll->Enabled = !StringGridIsEmpty(sgServer) && !StringGridIsEmpty
		(sgLocal);
	btnCopyDataMass->Enabled = btnCopyDataAll->Enabled;

	RestoreCursor();
}

// ---------------------------------------------------------------------------
int TMain::SetServerVan(int Index, TOracleVan * Van) {
	if (Index < 0) {
		if (!StringGridIsEmpty(sgServer)) {
			sgServer->RowCount++;
		}
		Index = sgServer->RowCount - 1;
	}

	sgServer->Objects[ServerColumns.VAN_OBJECT][Index] = Van;

	sgServer->Cells[ServerColumns.NUM][Index] = IntToStr(Index);

	sgServer->Cells[ServerColumns.VANNUM][Index] = Van->VanNum;

	sgServer->Cells[ServerColumns.CARGOTYPE][Index] = Van->CargoType;

	sgServer->Cells[ServerColumns.INVOICE_NUM][Index] = Van->InvoiceNum;

	sgServer->Cells[ServerColumns.INVOICE_SUPPLIER][Index] =
		Van->InvoiceSupplier;
	sgServer->Cells[ServerColumns.INVOICE_RECIPIENT][Index] =
		Van->InvoiceRecipient;

	sgServer->Cells[ServerColumns.DEPART_STATION][Index] = Van->DepartStation;
	sgServer->Cells[ServerColumns.PURPOSE_STATION][Index] = Van->PurposeStation;

	StringGridSetCellInt(sgServer, ServerColumns.CARRYING, Index,
		Van->Carrying);
	StringGridSetCellInt(sgServer, ServerColumns.TARE_T, Index, Van->TareT);
	StringGridSetCellInt(sgServer, ServerColumns.INVOICE_NETTO, Index,
		Van->InvoiceNetto);
	StringGridSetCellInt(sgServer, ServerColumns.INVOICE_TARE, Index,
		Van->InvoiceTare);

	return Index;
}

// ---------------------------------------------------------------------------
int TMain::SetLocalVan(int Index, TLocalVan * Van) {
	if (Index < 0) {
		if (!StringGridIsEmpty(sgLocal)) {
			sgLocal->RowCount++;
		}
		Index = sgLocal->RowCount - 1;
	}

	sgLocal->Objects[LocalColumns.VAN_OBJECT][Index] = Van;

	sgLocal->Cells[LocalColumns.NUM][Index] = IntToStr(Index);

	sgLocal->Cells[LocalColumns.DATETIME][Index] = DTToS(Van->DateTime);

	sgLocal->Cells[LocalColumns.VANNUM][Index] = Van->VanNum;

	sgLocal->Cells[LocalColumns.CARGOTYPE][Index] = Van->CargoType;

	sgLocal->Cells[LocalColumns.INVOICE_NUM][Index] = Van->InvoiceNum;

	sgLocal->Cells[LocalColumns.INVOICE_SUPPLIER][Index] = Van->InvoiceSupplier;
	sgLocal->Cells[LocalColumns.INVOICE_RECIPIENT][Index] =
		Van->InvoiceRecipient;

	sgLocal->Cells[LocalColumns.DEPART_STATION][Index] = Van->DepartStation;
	sgLocal->Cells[LocalColumns.PURPOSE_STATION][Index] = Van->PurposeStation;

	StringGridSetCellInt(sgLocal, LocalColumns.CARRYING, Index, Van->Carrying);
	StringGridSetCellInt(sgLocal, LocalColumns.TARE_T, Index, Van->TareT);
	StringGridSetCellInt(sgLocal, LocalColumns.INVOICE_NETTO, Index,
		Van->InvoiceNetto);
	StringGridSetCellInt(sgLocal, LocalColumns.INVOICE_TARE, Index,
		Van->InvoiceTare);

	StringGridSetCellInt(sgLocal, LocalColumns.NETTO_DIFF, Index,
		Van->NettoDiff);

	SetLocalVanTare(Index, !Van->IsLoaded);

	return Index;
}

// ---------------------------------------------------------------------------
TOracleVan * TMain::GetServerVan(int Index) {
	return (TOracleVan *) sgServer->Objects[ServerColumns.VAN_OBJECT][Index];
}

// ---------------------------------------------------------------------------
TLocalVan * TMain::GetLocalVan(int Index) {
	return (TLocalVan *) sgLocal->Objects[LocalColumns.VAN_OBJECT][Index];
}

// ---------------------------------------------------------------------------
bool IsRightTrainNum(String Value) {
	// check int num
	for (int i = 1; i < Value.Length(); i++) {
		if (Value[i] < '0' || Value[i] > '9') {
			return false;
		}
	}

	return true;
}

// ---------------------------------------------------------------------------
void TMain::SetOracleTrainNum(String Value) {
	FOracleTrainNum = Value;

	eRWNum->Text = OracleTrainNum;

	bool WithJoin = IsRightTrainNum(OracleTrainNum) && !IsShift();

	ServerLoadTrain(OracleTrainNum, WithJoin);
}

// ---------------------------------------------------------------------------
void TMain::SetLocalTrainNum(String Value) {
	if (FLocalTrainNum == Value) {
		return;
	}

	if (LocalChanged) {
		if (!MsgBoxYesNo(IDS_QUESTION_DATA_NEED_SAVE)) {
			return;
		}
	}

	FLocalTrainNum = Value;

	LocalLoadVans();
}

// ---------------------------------------------------------------------------
void TMain::SetDateLocal(TDate Value) {
	FDateLocal = Value;

	LocalLoadVans();
}

// ---------------------------------------------------------------------------
void TMain::SetServerVanList(TOracleVanList * Value) {
	if (Value == NULL) {
		ServerVanList->Clear();
	}
	else {
		ServerVanList->Assign(Value);
	}

	ServerSelectedRow = -1;

	StringGridClear(sgServer);

	ProcMess();

	for (int i = 0; i < ServerVanList->Count; i++) {
		SetServerVan(-1, ServerVanList->Items[i]);
	}

	ServerSelectedRow = StringGridIsEmpty(sgServer) ? -1 : 1;
}

// ---------------------------------------------------------------------------
void TMain::SetLocalVanList(TLocalVanList * Value) {
	if (Value == NULL) {
		LocalVanList->Clear();
	}
	else {
		LocalVanList->Assign(Value);
	}

	LocalSelectedRow = -1;

	StringGridClear(sgLocal);

	ProcMess();

	for (int i = 0; i < LocalVanList->Count; i++) {
		if (!IsEmpty(LocalTrainNum) || LocalVanList->Items[i]->IsLoaded) {
			SetLocalVan(-1, LocalVanList->Items[i]);
		}
	}

	LocalSelectedRow = StringGridIsEmpty(sgLocal) ? -1 : 1;
}

// ---------------------------------------------------------------------------
bool TMain::ServerLoadTrain(String TrainNum, bool WithJoin) {
	bool Result;

	String ResultMessage;

	StartDBOperation(dboLoad);

	ServerVanList = NULL;

	TDBOracleLoadTrain * DBOracleLoadTrain =
		new TDBOracleLoadTrain(Main->Settings->ServerOracleConnection, TrainNum,
		WithJoin);
	try {
		Result = DBOracleLoadTrain->Execute();

		ResultMessage = DBOracleLoadTrain->ErrorMessage;

		ServerVanList = DBOracleLoadTrain->VanList;
	}
	__finally {
		DBOracleLoadTrain->Free();

		EndDBOperation();
	}

	if (Result) {
		if (ServerVanList->Count == 0) {
			MsgBox(Format(IDS_MSG_RWNUM_NOT_EXISTS, TrainNum));
		}
	}
	else {
		MsgBoxErr(Format(IDS_ERROR_ORACLE_TRAIN_LOAD, ResultMessage));
	}

	return Result;
}

// ---------------------------------------------------------------------------
bool TMain::LocalLoadVans() {
	bool Result;

	String ResultMessage;

	StartDBOperation(dboLoad);

	LocalVanList = NULL;

	LocalChanged = false;

	TDBLocalLoadVans * DBLocalLoadVans =
		new TDBLocalLoadVans(Main->Settings->LocalConnection, DateLocal,
		LocalTrainNum);
	try {
		Result = DBLocalLoadVans->Execute();

		ResultMessage = DBLocalLoadVans->ErrorMessage;

		LocalVanList = DBLocalLoadVans->VanList;
	}
	__finally {
		DBLocalLoadVans->Free();

		EndDBOperation();
	}

	if (!Result) {
		MsgBoxErr(Format(IDS_ERROR_LOCAL_LOAD_VANS, ResultMessage));
	}

	return Result;
}

// ---------------------------------------------------------------------------
bool TMain::LocalSaveVanProps() {
	bool Result;

	String ResultMessage;

	StartDBOperation(dboSave);

	TDBLocalSaveVanProps * DBLocalSaveVanProps =
		new TDBLocalSaveVanProps(Main->Settings->LocalConnection,
		ServerVanList);
	try {
		Result = DBLocalSaveVanProps->Execute();

		ResultMessage = DBLocalSaveVanProps->ErrorMessage;
	}
	__finally {
		DBLocalSaveVanProps->Free();

		EndDBOperation();
	}

	if (!Result) {
		MsgBoxErr(Format(IDS_ERROR_LOCAL_SAVE_VANPROPS, ResultMessage));
	}

	return Result;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnServerLoadClick(TObject *Sender) {
	if (IsEmpty(eRWNum->Text)) {
		eRWNum->SetFocus();
		MsgBoxErr(IDS_ERROR_NEED_RWNUM);
		return;
	}

	OracleTrainNum = eRWNum->Text;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnLocalLoadClick(TObject *Sender) {
	if (!Settings->UseLocal) {
		return;
	}

	if (LocalChanged) {
		if (!MsgBoxYesNo(IDS_QUESTION_DATA_NEED_SAVE)) {
			return;
		}
	}

#ifdef _DEBUG
	if (true) {
		DateLocal = StrToDate("12.02.2019");
	}
	else {
		DateLocal = StrToDate("02.02.2021");
	}
#else
	DateLocal = Now();
#endif
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgServerFixedCellClick(TObject *Sender, int ACol,
	int ARow) {
	StringGridSelectRowAfterFixedCellClick((TStringGrid*)Sender, ARow);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnOptionsClick(TObject *Sender) {
#ifndef FORCELOGON
	if (TfrmLogin::Show(Settings->OptionsPass))
#endif
	{
		if (TfrmOptions::Show(Settings)) {
			if (!Settings->Load()) {
				MsgBoxErr(IDS_ERROR_LOAD_SETTINGS);

				Application->Terminate();
			}
			else {
				SetUseLocal();
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormResize(TObject *Sender) {
	if (!Visible) {
		return;
	}

	if (!Settings->UseLocal) {
		return;
	}

	TimerResize->Enabled = false;
	TimerResize->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::TimerResizeTimer(TObject *Sender) {
	TimerResize->Enabled = false;

	if (sgLocal->Height < Splitter->MinSize) {
		sgServer->Height = ClientHeight - Splitter->MinSize -
			PanelServer->Height - Splitter->Height - PanelCommon->Height -
			PanelLocal->Height - StatusBar->Height;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnServerTrainsClick(TObject *Sender) {
	frmServerTrains->Visible = !frmServerTrains->Visible;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnLocalTrainsClick(TObject *Sender) {
	frmLocalTrains->Visible = !frmLocalTrains->Visible;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnSaveVanPropsClick(TObject *Sender) {
	if (MsgBoxYesNo(IDS_QUESTION_SAVE_VANPROPS)) {
		LocalSaveVanProps();
	}
}

// ---------------------------------------------------------------------------
HWND TMain::AvitekGetProtFormBtn(TAvitekBtn AvitekBtn) {
	HWND hChild;

	int ToolBarIndex;
	int BtnIndex;

	String ControlText;
	String BtnControlText;

	int ErrorBtnNotFound;

	HWND hWnd = FindWindowByClass("TWVanProtForm");

	ToolBarIndex = 5;

	switch (AvitekBtn) {
	case abSave:
		BtnIndex = 2;
		BtnControlText = "��������� F7";
		ErrorBtnNotFound = IDS_LOG_ERROR_AVITEK_SAVE_BTN_NOT_FOUND;
		break;
	case abUpdate:
		BtnIndex = 4;
		BtnControlText = "�������� F5";
		ErrorBtnNotFound = IDS_LOG_ERROR_AVITEK_UPD_BTN_NOT_FOUND;
		break;
	}

	if (!hWnd) {
		hWnd = FindWindowByClass("TWTrainProtForm");

		ToolBarIndex = 3;
	}

	if (hWnd) {
#ifdef ENUM_AVITEK_CONTROLS
		TStringList * S = new TStringList();
		DebugEnumWindowControls(hWnd, S);
		for (int i = 0; i < S->Count; i++) {
			WriteToLog(S->Strings[i]);
		}
		S->Free();
#endif

		hChild = GetWindowChildByIndex(hWnd, ToolBarIndex);
		if (hChild) {
			hChild = GetWindowChildByIndex(hChild, BtnIndex);
			if (hChild) {
				ControlText = GetControlText(hChild);
			}
		}

		if (AnsiSameStr(ControlText, BtnControlText)) {
			return hChild;
		}
		else {
			WriteToLog(ErrorBtnNotFound);
		}
	}

	return 0;
}

// ---------------------------------------------------------------------------
bool TMain::AvitekCheckNeedSave() {
	HWND hButton = AvitekGetProtFormBtn(abSave);

	if (hButton) {
		if (IsWindowEnabled(hButton)) {
			WriteToLog(IDS_LOG_ERROR_AVITEK_NEED_SAVE);

			if (MsgBoxYesNo(IDS_ERROR_AVITEK_NEED_SAVE)) {
				SwitchToThisWindow(hButton, true);
			}

			return true;
		}
	}

	return false;
}

// ---------------------------------------------------------------------------
void TMain::AvitekUpdateProt() {
	HWND hButton = AvitekGetProtFormBtn(abUpdate);

	if (hButton) {
		if (IsWindowEnabled(hButton)) {
			SendMessage(hButton, BM_CLICK, (WPARAM)NULL, (LPARAM)NULL);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnCopyDataAllClick(TObject * Sender) {
	if (AvitekCheckNeedSave()) {
		return;
	}

	CopyData(((TButton *) Sender)->Tag == 0);
}

// ---------------------------------------------------------------------------
bool TMain::CheckField(int LocalColumn, int ServerColumn, int LocalIndex,
	int ServerIndex) {
	if (!IsEmpty(sgLocal->Cells[LocalColumn][LocalIndex])) {
		if (!AnsiSameStr(sgLocal->Cells[LocalColumn][LocalIndex],
			sgServer->Cells[ServerColumn][ServerIndex])) {
			return true;
		}
	}
	return false;
}

// ---------------------------------------------------------------------------
bool TMain::DataExists(TIntegerPairList * Result) {
	int ServerIndex;
	int LocalIndex;

	for (int i = 0; i < Result->Count; i++) {
		ServerIndex = Result->Items[i]->Int1->Value;
		LocalIndex = Result->Items[i]->Int2->Value;

		if (CheckField(LocalColumns.CARGOTYPE, ServerColumns.CARGOTYPE,
			LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(LocalColumns.INVOICE_NUM, ServerColumns.INVOICE_NUM,
			LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(LocalColumns.INVOICE_SUPPLIER,
			ServerColumns.INVOICE_SUPPLIER, LocalIndex, ServerIndex)) {
			return true;
		}
		if (CheckField(LocalColumns.INVOICE_RECIPIENT,
			ServerColumns.INVOICE_RECIPIENT, LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(LocalColumns.DEPART_STATION,
			ServerColumns.DEPART_STATION, LocalIndex, ServerIndex)) {
			return true;
		}
		if (CheckField(LocalColumns.PURPOSE_STATION,
			ServerColumns.PURPOSE_STATION, LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(LocalColumns.CARRYING, ServerColumns.CARRYING,
			LocalIndex, ServerIndex)) {
			return true;
		}

		// TODO: copy tare, update calc fields
		// if (CheckField(LocalColumns.TARE_T, ServerColumns.TARE_T,
		// Result->Items[LocalIndex]->Value, ServerIndex)) {
		// return true;
		// }

		if (CheckField(LocalColumns.INVOICE_NETTO, ServerColumns.INVOICE_NETTO,
			LocalIndex, ServerIndex)) {
			return true;
		}
		if (CheckField(LocalColumns.INVOICE_TARE, ServerColumns.INVOICE_TARE,
			LocalIndex, ServerIndex)) {
			return true;
		}
	}

	return false;
}

// ---------------------------------------------------------------------------
void TMain::CopyData(bool CopyAll) {
	TStringList * Source = new TStringList();
	TStringList * Dest = new TStringList();

	TIntegerPairList * Result = new TIntegerPairList();

	for (int i = 1; i < sgServer->RowCount; i++) {
		Source->Add(sgServer->Cells[ServerColumns.VANNUM][i]);
	}
	for (int i = 1; i < sgLocal->RowCount; i++) {
		if (GetLocalVan(i)->IsLoaded) {
			Dest->Add(sgLocal->Cells[LocalColumns.VANNUM][i]);
		} else {
			Dest->Add("*");
		}
	}

	try {
		FindMatch(Source, Dest, Result);

		if (Result->Count == 0) {
			WriteToLog(IDS_LOG_COPY_DATA_NOT_FOUND);
			MsgBox(IDS_MSG_MATCH_NOT_FOUND);
			return;
		}

		for (int i = 0; i < Result->Count; i++) {
			Result->Items[i]->Int1->Value++;
			Result->Items[i]->Int2->Value++;
		}

		TGridRect Selection;

		Selection.Left = LocalColumns.VANNUM;
		Selection.Right = LocalColumns.VANNUM;

		int MinInt2 = Result->Items[0]->Int2->Value;
		int MaxInt2 = Result->Items[0]->Int2->Value;
		for (int i = 0; i < Result->Count; i++) {
			if (Result->Items[i]->Int2->Value < MinInt2) {
				MinInt2 = Result->Items[i]->Int2->Value;
			}
			if (Result->Items[i]->Int2->Value > MaxInt2) {
				MaxInt2 = Result->Items[i]->Int2->Value;
			}
		}
		Selection.Top = MinInt2;
		if (MaxInt2 - MinInt2 > Result->Count) {
			Selection.Bottom = Selection.Top;
		}
		else {
			Selection.Bottom = MaxInt2;
		}

		int OldLocalSelectedRow = sgLocal->Row;

		sgLocal->Row = Selection.Top;
		sgLocal->Selection = Selection;

		LocalSelectedRow = sgLocal->Row;

		StringGridInvalidateCell(sgLocal, 0, OldLocalSelectedRow);
		StringGridInvalidateCell(sgLocal, 0, LocalSelectedRow);

		if (CopyAll) {
			if (DataExists(Result)) {
				if (!MsgBoxYesNo(IDS_QUESTION_DATA_OVERWRITE)) {
					WriteToLog(IDS_LOG_COPY_DATA_OVERWRITE_CANCEL);
					return;
				}
			}
		}

		TOracleVan * ServerVan;
		TLocalVan * LocalVan;

		int ServerIndex;
		int LocalIndex;

		for (int i = 0; i < Result->Count; i++) {
			ServerIndex = Result->Items[i]->Int1->Value;
			LocalIndex = Result->Items[i]->Int2->Value;

			ServerVan = GetServerVan(ServerIndex);
			LocalVan = GetLocalVan(LocalIndex);

			LocalVan->VanNum = ServerVan->VanNum;

			if (CopyAll) {
				LocalVan->CargoType = ServerVan->CargoType;

				LocalVan->InvoiceNum = ServerVan->InvoiceNum;

				LocalVan->InvoiceSupplier = ServerVan->InvoiceSupplier;
				LocalVan->InvoiceRecipient = ServerVan->InvoiceRecipient;

				LocalVan->DepartStation = ServerVan->DepartStation;
				LocalVan->PurposeStation = ServerVan->PurposeStation;
			}

			LocalVan->Carrying = ServerVan->Carrying;

			// TODO: copy tare, update calc fields
			// sgLocal->Cells[LocalColumns.TARE_T][LocalIndex] = ServerVan->TareT;
			// if (true) { // TODO: check tare type
			// if (IsEmpty(sgLocal->Cells[LocalColumns.TARE_T][LocalIndex])) {
			// sgLocal->Cells[LocalColumns.NETTO][LocalIndex] = " ";
			// }
			// else {
			// Brutto =
			// StrToInt(sgLocal->Cells[LocalColumns.BRUTTO]
			// [LocalIndex]);
			// TareT = StrToInt
			// (sgLocal->Cells[LocalColumns.TARE_T][LocalIndex]);
			//
			// sgLocal->Cells[LocalColumns.NETTO][LocalIndex] =
			// IntToStr(Brutto - TareT);
			// }
			// }

			LocalVan->InvoiceNetto = ServerVan->InvoiceNetto;
			LocalVan->InvoiceTare = ServerVan->InvoiceTare;

			SetLocalVan(LocalIndex, LocalVan);

			SetLocalVanChanged(LocalIndex, true);
		}

		WriteToLog(Format(IDS_LOG_COPY_DATA, ARRAYOFCONST((Result->Count))));
	}
	__finally {
		Result->Free();
		Dest->Free();
		Source->Free();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgServerSelectCell(TObject *Sender, int ACol, int ARow,
	bool &CanSelect) {
	if (StringGridIsEmpty(sgServer)) {
		return;
	}

	if (ServerSelectedRow == ARow) {
		return;
	}

	StringGridInvalidateCell(sgServer, 0, ServerSelectedRow);

	ServerSelectedRow = ARow;

	StringGridInvalidateCell(sgServer, 0, ServerSelectedRow);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgLocalSelectCell(TObject *Sender, int ACol, int ARow,
	bool &CanSelect) {
	if (StringGridIsEmpty(sgLocal)) {
		return;
	}

	if (LocalSelectedRow == ARow) {
		return;
	}

	StringGridInvalidateCell(sgLocal, 0, LocalSelectedRow);

	LocalSelectedRow = ARow;

	StringGridInvalidateCell(sgLocal, 0, LocalSelectedRow);
}

// ---------------------------------------------------------------------------
void TMain::SetLocalVanChanged(int Index, bool Changed) {
	if (IsLocalVanChanged(Index) == Changed) {
		return;
	}

	if (Changed) {
		sgLocal->Cells[LocalColumns.CHANGED][Index] = "*";
		LocalChanged = true;
	}
	else {
		sgLocal->Cells[LocalColumns.CHANGED][Index] = "";
	}

	StringGridInvalidateCell(sgLocal, 0, Index);
}

// ---------------------------------------------------------------------------
bool TMain::IsLocalVanChanged(int Index) {
	return sgLocal->Cells[LocalColumns.CHANGED][Index] == "*";
}

// ---------------------------------------------------------------------------
void TMain::SetLocalVanTare(int Index, bool Tare) {
	if (IsLocalVanTare(Index) == Tare) {
		return;
	}

	if (Tare) {
		sgLocal->Cells[LocalColumns.WEIGHT_TYPE_TARE][Index] = "*";
	}
	else {
		sgLocal->Cells[LocalColumns.WEIGHT_TYPE_TARE][Index] = "";
	}
}

// ---------------------------------------------------------------------------
bool TMain::IsLocalVanTare(int Index) {
	return sgLocal->Cells[LocalColumns.WEIGHT_TYPE_TARE][Index] == "*";
}

// ---------------------------------------------------------------------------
void TMain::SetLocalChanged(bool Changed) {
	if (FLocalChanged == Changed) {
		return;
	}

	FLocalChanged = Changed;

	btnLocalSave->Enabled = Changed;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnLocalSaveClick(TObject * Sender) {
	if (AvitekCheckNeedSave()) {
		return;
	}

	if (LocalSaveVans()) {
		AvitekUpdateProt();
	}
}

// ---------------------------------------------------------------------------
bool TMain::LocalSaveVans() {
	bool Result = false;

	String ResultMessage;

	int SaveCount = 0;

	StartDBOperation(dboSave);

	TDBLocalSaveVan * DBLocalSaveVan =
		new TDBLocalSaveVan(Main->Settings->LocalConnection);
	try {
		for (int i = 1; i < sgLocal->RowCount; i++) {
			if (!IsLocalVanChanged(i)) {
				continue;
			}

			DBLocalSaveVan->Van = GetLocalVan(i);

			Result = DBLocalSaveVan->Execute();

			ResultMessage = DBLocalSaveVan->ErrorMessage;

			if (Result) {
				SaveCount++;

				SetLocalVanChanged(i, false);
			}
			else {
				break;
			}
		}

		if (Result) {
			LocalChanged = false;
		}
	}
	__finally {
		DBLocalSaveVan->Free();

		EndDBOperation();

		if (SaveCount > 0) {
			WriteToLog(Format(IDS_LOG_LOCAL_SAVE_VANS_OK,
				ARRAYOFCONST((SaveCount))));
		}
	}

	if (!Result) {
		MsgBoxErr(Format(IDS_ERROR_LOCAL_SAVE_VANS, ResultMessage));
	}

	return Result;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgLocalDblClick(TObject *Sender) {
#ifdef _DEBUG
	if (LocalSelectedRow > 0) {
		TLocalVan * Van = GetLocalVan(LocalSelectedRow);
		MsgBox("VanNum: " + Van->VanNum + sLineBreak + "Brutto: " +
			Van->Brutto + sLineBreak + "TareT: " + Van->TareT + sLineBreak +
			"Netto: " + Van->Netto + sLineBreak + "Carrying: " + Van->Carrying +
			sLineBreak + "Overload: " + Van->Overload);
	}
#endif
}
// ---------------------------------------------------------------------------
