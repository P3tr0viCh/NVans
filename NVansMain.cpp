// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Clipbrd.hpp"

#include <AboutFrm.h>

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsKAndM.h>
#include <UtilsFiles.h>
#include <UtilsFileIni.h>
#include <UtilsStringGrid.h>

#include "NVansDebug.h"

#include "NVansAdd.h"
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

// ---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormCreate(TObject *Sender) {
	WriteToLogProgramStart();

	FSettings = new TSettings();

	ServerColumns = new TNVansServerColumns();
	LocalColumns = new TNVansLocalColumns();

	FOracleTrainNum = "";

	ServerSelectedRow = -1;
	LocalSelectedRow = -1;

	FServerVanList = new TOracleVanList();
	FLocalVanList = new TLocalVanList();

	Caption = Application->Title + " " + GetFileVer(Application->ExeName);

	DefaultRowHeight = Canvas->TextHeight("ComboBox") + 8;

	StringGridInit(sgServer, ServerColumns, DefaultRowHeight);
	StringGridInit(sgLocal, LocalColumns, DefaultRowHeight);

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

	LocalColumns->Free();
	ServerColumns->Free();

	FSettings->Free();

	WriteToLogProgramStop();
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
void __fastcall TMain::btnAboutClick(TObject *Sender) {
	ShowAbout(18, MAXBYTE, MAXBYTE, MAXBYTE, NULL, NULL, NULL, NULL, NULL,
		LoadStr(IDS_COPYRIGHT));
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
void __fastcall TMain::ApplicationEventsException(TObject *Sender, Exception *E)
{
	WriteToLog(Format(IDS_LOG_ERROR_EXCEPTION, E->Message));
	MsgBoxErr(Format(IDS_ERROR_UNKNOWN_EXCEPTION, E->Message));
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgServerDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State) {
	StringGridDrawCell(sgServer, ACol, ARow, Rect, State, NUSet,
		ServerColumns->LeftAlign, NUSet, Main->Settings->ColorReadOnly, NUColor,
		true, false, NUColor, true, Main->Settings->ColorSelected);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgLocalDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State) {
	StringGridDrawCell(sgLocal, ACol, ARow, Rect, State, IsLocalVanTare(ARow) ?
		LocalColumns->ReadOnlyIfTare : NUSet, LocalColumns->LeftAlign, NUSet,
		Main->Settings->ColorReadOnly, NUColor, true, false,
		Main->Settings->ColorChanged, true, Main->Settings->ColorSelected);
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
		Index = StringGridAddRow(sgServer);
	}

	sgServer->Objects[TNVansServerColumns::VAN_OBJECT][Index] = Van;

	sgServer->Cells[TNVansServerColumns::NUM][Index] = IntToStr(Index);

	sgServer->Cells[TNVansServerColumns::VANNUM][Index] = Van->VanNum;

	sgServer->Cells[TNVansServerColumns::CARGOTYPE][Index] = Van->CargoType;

	sgServer->Cells[TNVansServerColumns::INVOICE_NUM][Index] = Van->InvoiceNum;

	sgServer->Cells[TNVansServerColumns::INVOICE_SUPPLIER][Index] =
		Van->InvoiceSupplier;
	sgServer->Cells[TNVansServerColumns::INVOICE_RECIPIENT][Index] =
		Van->InvoiceRecipient;

	sgServer->Cells[TNVansServerColumns::DEPART_STATION][Index] =
		Van->DepartStation;
	sgServer->Cells[TNVansServerColumns::PURPOSE_STATION][Index] =
		Van->PurposeStation;

	StringGridSetCellInt(sgServer, TNVansServerColumns::CARRYING, Index,
		Van->Carrying);
	StringGridSetCellInt(sgServer, TNVansServerColumns::TARE_T, Index,
		Van->TareT);
	StringGridSetCellInt(sgServer, TNVansServerColumns::INVOICE_NETTO, Index,
		Van->InvoiceNetto);
	StringGridSetCellInt(sgServer, TNVansServerColumns::INVOICE_TARE, Index,
		Van->InvoiceTare);

	return Index;
}

// ---------------------------------------------------------------------------
int TMain::SetLocalVan(int Index, TLocalVan * Van) {
	if (Index < 0) {
		Index = StringGridAddRow(sgLocal);
	}

	sgLocal->Objects[TNVansLocalColumns::VAN_OBJECT][Index] = Van;

	sgLocal->Cells[TNVansLocalColumns::NUM][Index] = IntToStr(Index);

	sgLocal->Cells[TNVansLocalColumns::DATETIME][Index] = DTToS(Van->DateTime);

	sgLocal->Cells[TNVansLocalColumns::VANNUM][Index] = Van->VanNum;

	sgLocal->Cells[TNVansLocalColumns::CARGOTYPE][Index] = Van->CargoType;

	sgLocal->Cells[TNVansLocalColumns::INVOICE_NUM][Index] = Van->InvoiceNum;

	sgLocal->Cells[TNVansLocalColumns::INVOICE_SUPPLIER][Index] =
		Van->InvoiceSupplier;
	sgLocal->Cells[TNVansLocalColumns::INVOICE_RECIPIENT][Index] =
		Van->InvoiceRecipient;

	sgLocal->Cells[TNVansLocalColumns::DEPART_STATION][Index] =
		Van->DepartStation;
	sgLocal->Cells[TNVansLocalColumns::PURPOSE_STATION][Index] =
		Van->PurposeStation;

	StringGridSetCellInt(sgLocal, TNVansLocalColumns::CARRYING, Index,
		Van->Carrying);
	StringGridSetCellInt(sgLocal, TNVansLocalColumns::TARE_T, Index,
		Van->TareT);
	StringGridSetCellInt(sgLocal, TNVansLocalColumns::INVOICE_NETTO, Index,
		Van->InvoiceNetto);
	StringGridSetCellInt(sgLocal, TNVansLocalColumns::INVOICE_TARE, Index,
		Van->InvoiceTare);

	StringGridSetCellInt(sgLocal, TNVansLocalColumns::NETTO_DIFF, Index,
		Van->NettoDiff);

	return Index;
}

// ---------------------------------------------------------------------------
TOracleVan * TMain::GetServerVan(int Index) {
	return (TOracleVan *) sgServer->Objects
		[TNVansServerColumns::VAN_OBJECT][Index];
}

// ---------------------------------------------------------------------------
TLocalVan * TMain::GetLocalVan(int Index) {
	return (TLocalVan *) sgLocal->Objects[TNVansLocalColumns::VAN_OBJECT]
		[Index];
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
		BtnControlText = "Сохранить F7";
		ErrorBtnNotFound = IDS_LOG_ERROR_AVITEK_SAVE_BTN_NOT_FOUND;
		break;
	case abUpdate:
		BtnIndex = 4;
		BtnControlText = "Обновить F5";
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

		if (CheckField(TNVansLocalColumns::CARGOTYPE,
			TNVansServerColumns::CARGOTYPE, LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(TNVansLocalColumns::INVOICE_NUM,
			TNVansServerColumns::INVOICE_NUM, LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(TNVansLocalColumns::INVOICE_SUPPLIER,
			TNVansServerColumns::INVOICE_SUPPLIER, LocalIndex, ServerIndex)) {
			return true;
		}
		if (CheckField(TNVansLocalColumns::INVOICE_RECIPIENT,
			TNVansServerColumns::INVOICE_RECIPIENT, LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(TNVansLocalColumns::DEPART_STATION,
			TNVansServerColumns::DEPART_STATION, LocalIndex, ServerIndex)) {
			return true;
		}
		if (CheckField(TNVansLocalColumns::PURPOSE_STATION,
			TNVansServerColumns::PURPOSE_STATION, LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(TNVansLocalColumns::CARRYING,
			TNVansServerColumns::CARRYING, LocalIndex, ServerIndex)) {
			return true;
		}

		// TODO: copy tare, update calc fields
		// if (CheckField(LocalColumns.TARE_T, ServerColumns.TARE_T,
		// Result->Items[LocalIndex]->Value, ServerIndex)) {
		// return true;
		// }

		if (CheckField(TNVansLocalColumns::INVOICE_NETTO,
			TNVansServerColumns::INVOICE_NETTO, LocalIndex, ServerIndex)) {
			return true;
		}
		if (CheckField(TNVansLocalColumns::INVOICE_TARE,
			TNVansServerColumns::INVOICE_TARE, LocalIndex, ServerIndex)) {
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
		Source->Add(sgServer->Cells[TNVansServerColumns::VANNUM][i]);
	}
	for (int i = 1; i < sgLocal->RowCount; i++) {
		if (GetLocalVan(i)->IsLoaded) {
			Dest->Add(sgLocal->Cells[TNVansLocalColumns::VANNUM][i]);
		}
		else {
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

		Selection.Left = TNVansLocalColumns::VANNUM;
		Selection.Right = TNVansLocalColumns::VANNUM;

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

			StringGridRowSetChanged(sgLocal, LocalIndex, true);
		}

		LocalChanged = true;

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
bool TMain::IsLocalVanTare(int Index) {
	TLocalVan * LocalVan = GetLocalVan(Index);

	if (LocalVan == NULL) {
		return false;
	}

	return !LocalVan->IsLoaded;
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
			if (!StringGridRowIsChanged(sgLocal, i)) {
				continue;
			}

			DBLocalSaveVan->Van = GetLocalVan(i);

			Result = DBLocalSaveVan->Execute();

			ResultMessage = DBLocalSaveVan->ErrorMessage;

			if (Result) {
				SaveCount++;

				StringGridRowSetChanged(sgLocal, i, false);
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
