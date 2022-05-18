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

#include "NVansTDBOracleLoadTrain.h"
#include "NVansTDBLocalLoadVans.h"
#include "NVansTDBLocalSaveVanProps.h"

#include "NVansLogin.h"
#include "NVansOptions.h"
#include "NVansServerList.h"

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

	FTrainNum = "";

	FServerVanList = new TVanList();
	FLocalVanList = new TVanList();

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
		FileIni->ReadFormBounds(this);

		sgServer->Height = FileIni->ReadInteger(Name, "ServerHeight",
			sgServer->Height);

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
	CanClose = MsgBoxYesNo(IDS_QUESTION_CLOSE_PROGRAM, true,
		Application->Handle);
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
		true, false, false, NUColor);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgLocalDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State) {
	StringGridDrawCell(sgLocal, ACol, ARow, Rect, State, LocalColumns.ReadOnly,
		LocalColumns.LeftAlign, NUSet, Main->Settings->ColorReadOnly, NUColor,
		true, false, LocalChanged.find(ARow) != LocalChanged.end(),
		Main->Settings->ColorChanged);
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
void TMain::StartLoad() {
	ShowWaitCursor();
	StatusBar->SimpleText = LoadStr(IDS_STATUS_TRAIN_LOAD);
	SetControlsEnabled(false);
	frmServerList->StartLoad();
	ProcMess();
}

// ---------------------------------------------------------------------------
void TMain::EndLoad() {
	frmServerList->EndLoad();
	SetControlsEnabled(true);
	StatusBar->SimpleText = "";

	btnSaveVanProps->Enabled = !StringGridIsEmpty(sgServer);
	btnCopyData->Enabled = !StringGridIsEmpty(sgServer) && !StringGridIsEmpty
		(sgLocal);

	RestoreCursor();
}

// ---------------------------------------------------------------------------
int TMain::SetServerVan(int Index, TVan * Van) {
	if (Index < 0) {
		if (!StringGridIsEmpty(sgServer)) {
			sgServer->RowCount++;
		}
		Index = sgServer->RowCount - 1;
	}

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

	if (Van->Carrying > 0) {
		sgServer->Cells[ServerColumns.CARRYING][Index] =
			IntToStr(Van->Carrying);
	}
	if (Van->TareT > 0) {
		sgServer->Cells[ServerColumns.TARE_T][Index] = IntToStr(Van->TareT);
	}
	if (Van->InvoiceNetto > 0) {
		sgServer->Cells[ServerColumns.INVOICE_NETTO][Index] =
			IntToStr(Van->InvoiceNetto);
	}
	if (Van->InvoiceTare > 0) {
		sgServer->Cells[ServerColumns.INVOICE_TARE][Index] =
			IntToStr(Van->InvoiceTare);
	}

	return Index;
}

// ---------------------------------------------------------------------------
int TMain::SetLocalVan(int Index, TVan * Van) {
	if (Index < 0) {
		if (!StringGridIsEmpty(sgLocal)) {
			sgLocal->RowCount++;
		}
		Index = sgLocal->RowCount - 1;
	}

	sgLocal->Cells[LocalColumns.ID][Index] = IntToStr(Van->ID);

	sgLocal->Cells[LocalColumns.NUM][Index] = IntToStr(Index);

	sgLocal->Cells[LocalColumns.DATETIME][Index] = DateTimeToStr(Van->DateTime);

	sgLocal->Cells[LocalColumns.VANNUM][Index] = Van->VanNum;

	sgLocal->Cells[LocalColumns.CARGOTYPE][Index] = Van->CargoType;

	sgLocal->Cells[LocalColumns.INVOICE_NUM][Index] = Van->InvoiceNum;

	sgLocal->Cells[LocalColumns.INVOICE_SUPPLIER][Index] = Van->InvoiceSupplier;
	sgLocal->Cells[LocalColumns.INVOICE_RECIPIENT][Index] =
		Van->InvoiceRecipient;

	sgLocal->Cells[LocalColumns.DEPART_STATION][Index] = Van->DepartStation;
	sgLocal->Cells[LocalColumns.PURPOSE_STATION][Index] = Van->PurposeStation;

	if (Van->Carrying > 0) {
		sgLocal->Cells[LocalColumns.CARRYING][Index] = IntToStr(Van->Carrying);
	}
	if (Van->TareT > 0) {
		sgLocal->Cells[LocalColumns.TARE_T][Index] = IntToStr(Van->TareT);
	}
	if (Van->InvoiceNetto > 0) {
		sgLocal->Cells[LocalColumns.INVOICE_NETTO][Index] =
			IntToStr(Van->InvoiceNetto);
	}
	if (Van->InvoiceTare > 0) {
		sgLocal->Cells[LocalColumns.INVOICE_TARE][Index] =
			IntToStr(Van->InvoiceTare);
	}

	sgLocal->Cells[LocalColumns.NETTO][Index] = IntToStr(Van->Netto);

	LocalUpdateCalcFields(Index);

	return Index;
}

// ---------------------------------------------------------------------------
int TMain::LocalFindVanByID(int ID) {
	for (int i = 0; i < LocalVanList->Count; i++) {
		if (LocalVanList->Items[i]->ID == ID) {
			return i;
		}
	}

	return -1;
}

// ---------------------------------------------------------------------------
TVan * TMain::GetLocalVan(int Index) {
	return NULL;
}

// ---------------------------------------------------------------------------
void TMain::LocalUpdateCalcFields(int Index) {
	if (IsEmpty(sgLocal->Cells[LocalColumns.NETTO][Index])) {
		return;
	}
	if (IsEmpty(sgLocal->Cells[LocalColumns.INVOICE_NETTO][Index])) {
		return;
	}

	int Netto = StrToInt(sgLocal->Cells[LocalColumns.NETTO][Index]);
	int InvoiceNetto =
		StrToInt(sgLocal->Cells[LocalColumns.INVOICE_NETTO][Index]);

	if (InvoiceNetto > 0) {
		sgLocal->Cells[LocalColumns.NETTO_DIFF][Index] =
			IntToStr(Netto - InvoiceNetto);
	}
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
void TMain::SetTrainNum(String Value) {
	FTrainNum = Value;

	eRWNum->Text = TrainNum;

	bool WithJoin = IsRightTrainNum(TrainNum) && !IsShift();

	ServerLoadTrain(TrainNum, WithJoin);
}

// ---------------------------------------------------------------------------
void TMain::SetDateLocal(TDate Value) {
	FDateLocal = Value;

	LocalLoadVans();
}

// ---------------------------------------------------------------------------
void TMain::SetServerVanList(TVanList * Value) {
	if (Value == NULL) {
		ServerVanList->Clear();
	}
	else {
		ServerVanList->Assign(Value);
	}

	StringGridClear(sgServer);

	ProcMess();

	for (int i = 0; i < ServerVanList->Count; i++) {
		SetServerVan(-1, ServerVanList->Items[i]);
	}
}

// ---------------------------------------------------------------------------
void TMain::SetLocalVanList(TVanList * Value) {
	if (Value == NULL) {
		LocalVanList->Clear();
	}
	else {
		LocalVanList->Assign(Value);
	}

	StringGridClear(sgLocal);

	ProcMess();

	for (int i = 0; i < LocalVanList->Count; i++) {
		if (LocalVanList->Items[i]->IsLoaded) {
			SetLocalVan(-1, LocalVanList->Items[i]);
		}
	}
}

// ---------------------------------------------------------------------------
bool TMain::ServerLoadTrain(String TrainNum, bool WithJoin) {
	bool Result;

	String ResultMessage;

	StartLoad();

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

		EndLoad();
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

	StartLoad();

	LocalVanList = NULL;

	LocalChanged.clear();

	TDBLocalLoadVans * DBLocalLoadVans =
		new TDBLocalLoadVans(Main->Settings->LocalConnection, DateLocal);
	try {
		Result = DBLocalLoadVans->Execute();

		ResultMessage = DBLocalLoadVans->ErrorMessage;

		LocalVanList = DBLocalLoadVans->VanList;
	}
	__finally {
		DBLocalLoadVans->Free();

		EndLoad();
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

	StartLoad();

	TDBLocalSaveVanProps * DBLocalSaveVanProps =
		new TDBLocalSaveVanProps(Main->Settings->LocalConnection,
		ServerVanList);
	try {
		Result = DBLocalSaveVanProps->Execute();

		ResultMessage = DBLocalSaveVanProps->ErrorMessage;
	}
	__finally {
		DBLocalSaveVanProps->Free();

		EndLoad();
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

	TrainNum = eRWNum->Text;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnLocalLoadClick(TObject *Sender) {
	if (!Settings->UseLocal) {
		return;
	}

	if (!LocalChanged.empty()) {
		if (!MsgBoxYesNo(IDS_QUESTION_DATA_NEED_SAVE)) {
			return;
		}
	}

#ifdef _DEBUG
	DateLocal = StrToDate("02.02.2021");
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
	if (Splitter->Top >= ClientHeight - 112) {
		sgServer->Height = ClientHeight - 224;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnServerListClick(TObject *Sender) {
	frmServerList->Visible = !frmServerList->Visible;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnSaveVanPropsClick(TObject *Sender) {
	if (MsgBoxYesNo(IDS_QUESTION_SAVE_VANPROPS)) {
		LocalSaveVanProps();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnCopyDataClick(TObject *Sender) {
	CopyData();
}

// ---------------------------------------------------------------------------
void TMain::CopyData() {
	TStringList * Source = new TStringList();
	TStringList * Dest = new TStringList();

	for (int i = 1; i < sgServer->RowCount; i++) {
		Source->Add(sgServer->Cells[ServerColumns.VANNUM][i]);
	}
	for (int i = 1; i < sgLocal->RowCount; i++) {
		Dest->Add(sgLocal->Cells[LocalColumns.VANNUM][i]);
	}

	bool Reverse;

	int FindMatchResult = FIND_MATCH_RESULT_NOT_FOUND;

	try {
		FindMatchResult = FindMatch(Source, Dest, Reverse);
	}
	__finally {
		Dest->Free();
		Source->Free();
	}

	// ---
	if (FindMatchResult <= FIND_MATCH_RESULT_NOT_FOUND) {
		MsgBox(IDS_MSG_MATCH_NOT_FOUND);
		return;
	}

	int Index = FindMatchResult + 1;

	TGridRect Selection;

	Selection.Left = LocalColumns.VANNUM;
	Selection.Right = LocalColumns.VANNUM;
	if (Reverse) {
		Selection.Bottom = Index;
		Selection.Top = Selection.Bottom - sgServer->RowCount + 2;
	}
	else {
		Selection.Top = Index;
		Selection.Bottom = Selection.Top + sgServer->RowCount - 2;
	}

	sgLocal->Selection = Selection;

	if (DataExists(Index, Reverse)) {
		if (!MsgBoxYesNo(IDS_QUESTION_DATA_OVERWRITE)) {
			return;
		}
	}

	for (int i = 1; i < sgServer->RowCount; i++) {
		sgLocal->Cells[LocalColumns.VANNUM][Index] =
			sgServer->Cells[ServerColumns.VANNUM][i];

		sgLocal->Cells[LocalColumns.CARGOTYPE][Index] =
			sgServer->Cells[ServerColumns.CARGOTYPE][i];

		sgLocal->Cells[LocalColumns.INVOICE_NUM][Index] =
			sgServer->Cells[ServerColumns.INVOICE_NUM][i];

		sgLocal->Cells[LocalColumns.INVOICE_SUPPLIER][Index] =
			sgServer->Cells[ServerColumns.INVOICE_SUPPLIER][i];
		sgLocal->Cells[LocalColumns.INVOICE_RECIPIENT][Index] =
			sgServer->Cells[ServerColumns.INVOICE_RECIPIENT][i];

		sgLocal->Cells[LocalColumns.DEPART_STATION][Index] =
			sgServer->Cells[ServerColumns.DEPART_STATION][i];
		sgLocal->Cells[LocalColumns.PURPOSE_STATION][Index] =
			sgServer->Cells[ServerColumns.PURPOSE_STATION][i];

		sgLocal->Cells[LocalColumns.CARRYING][Index] =
			sgServer->Cells[ServerColumns.CARRYING][i];
		sgLocal->Cells[LocalColumns.TARE_T][Index] =
			sgServer->Cells[ServerColumns.TARE_T][i];
		sgLocal->Cells[LocalColumns.INVOICE_NETTO][Index] =
			sgServer->Cells[ServerColumns.INVOICE_NETTO][i];
		sgLocal->Cells[LocalColumns.INVOICE_TARE][Index] =
			sgServer->Cells[ServerColumns.INVOICE_TARE][i];

		LocalUpdateCalcFields(Index);

		LocalChanged.insert(Index);

		if (Reverse) {
			Index--;
		}
		else {
			Index++;
		}
	}

	TRect Rect;
	for (TIntSet::iterator it = LocalChanged.begin();
	it != LocalChanged.end(); it++) {
		Rect = sgLocal->CellRect(0, *it);
		InvalidateRect(sgLocal->Handle, &Rect, false);
	}
}

// ---------------------------------------------------------------------------
bool TMain::CheckField(int Column1, int Column2, int Index1, int Index2) {
	if (!IsEmpty(sgLocal->Cells[Column1][Index1])) {
		if (!AnsiSameStr(sgLocal->Cells[Column1][Index1],
			sgServer->Cells[Column2][Index2])) {
			return true;
		}
	}
	return false;
}

// ---------------------------------------------------------------------------
bool TMain::DataExists(int Index, bool Reverse) {
	for (int i = 1; i < sgServer->RowCount; i++) {
		if (CheckField(LocalColumns.CARGOTYPE, ServerColumns.CARGOTYPE,
			Index, i)) {
			return true;
		}

		if (CheckField(LocalColumns.INVOICE_NUM, ServerColumns.INVOICE_NUM,
			Index, i)) {
			return true;
		}

		if (CheckField(LocalColumns.INVOICE_SUPPLIER,
			ServerColumns.INVOICE_SUPPLIER, Index, i)) {
			return true;
		}
		if (CheckField(LocalColumns.INVOICE_RECIPIENT,
			ServerColumns.INVOICE_RECIPIENT, Index, i)) {
			return true;
		}

		if (CheckField(LocalColumns.DEPART_STATION,
			ServerColumns.DEPART_STATION, Index, i)) {
			return true;
		}
		if (CheckField(LocalColumns.PURPOSE_STATION,
			ServerColumns.PURPOSE_STATION, Index, i)) {
			return true;
		}

		if (CheckField(LocalColumns.CARRYING, ServerColumns.CARRYING, Index, i))
		{
			return true;
		}
		if (CheckField(LocalColumns.TARE_T, ServerColumns.TARE_T, Index, i)) {
			return true;
		}
		if (CheckField(LocalColumns.INVOICE_NETTO, ServerColumns.INVOICE_NETTO,
			Index, i)) {
			return true;
		}
		if (CheckField(LocalColumns.INVOICE_TARE, ServerColumns.INVOICE_TARE,
			Index, i)) {
			return true;
		}

		if (Reverse) {
			Index--;
		}
		else {
			Index++;
		}
	}

	return false;
}
// ---------------------------------------------------------------------------
