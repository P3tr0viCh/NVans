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

	FServerVanList = new TOracleVanList();

	if (!Settings->Load()) {
		MsgBoxErr(IDS_ERROR_LOAD_SETTINGS);

		Application->Terminate();

		return;
	}

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

	// TODO
#ifdef _DEBUG
	eRWNum->Text = "42";
#else
	sgLocal->Visible = false;
	Splitter->Visible = false;
	sgServer->Align = alClient;
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
	CanClose = MsgBoxYesNo(IDS_QUESTION_CLOSE_PROGRAM);
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

	StringGridSetHeader(sgServer, ServerColumns.NUM, IDS_GRID_HEADER_NUM, 50);

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
		IDS_GRID_HEADER_CARRYING, 80);
	StringGridSetHeader(sgServer, ServerColumns.TARE_T,
		IDS_GRID_HEADER_TARE_T, 80);
}

// ---------------------------------------------------------------------------
void TMain::CreateLocalColumns() {
	sgLocal->ColCount = LocalColumns.VISIBLE_COUNT;

	StringGridSetHeader(sgLocal, LocalColumns.NUM, IDS_GRID_HEADER_NUM, 50);
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
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgServerDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State) {
	StringGridDrawCell(sgServer, ACol, ARow, Rect, State, NUSet,
		ServerColumns.LeftAlign, NUSet, Main->Settings->ColorReadOnly,
		NUColor, true);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgLocalDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State) {
	StringGridDrawCell(sgLocal, ACol, ARow, Rect, State, LocalColumns.ReadOnly,
		LocalColumns.LeftAlign, NUSet, Main->Settings->ColorReadOnly,
		NUColor, true);
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
	eRWNum->Enabled = Enabled;
	btnServerLoad->Enabled = Enabled;
	btnServerList->Enabled = Enabled;

	btnOptions->Enabled = Enabled;

	sgServer->Enabled = Enabled;
	Splitter->Enabled = Enabled;
	sgLocal->Enabled = Enabled;
}

// ---------------------------------------------------------------------------
void TMain::StartLoad() {
	StatusBar->SimpleText = LoadStr(IDS_STATUS_TRAIN_LOAD);
	SetControlsEnabled(false);
	frmServerList->StartLoad();
}

// ---------------------------------------------------------------------------
void TMain::EndLoad() {
	frmServerList->EndLoad();
	SetControlsEnabled(true);
	StatusBar->SimpleText = "";
}

// ---------------------------------------------------------------------------
int TMain::SetServerVan(int Index, TOracleVan * Van) {
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

	sgServer->Cells[ServerColumns.CARRYING][Index] = IntToStr(Van->Carrying);
	sgServer->Cells[ServerColumns.TARE_T][Index] = IntToStr(Van->TareT);

	return Index;
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

	LoadTrain(TrainNum, WithJoin);
}

// ---------------------------------------------------------------------------
void TMain::SetServerVanList(TOracleVanList * Value) {
	if (Value == NULL) {
		ServerVanList->Clear();
	}
	else {
		ServerVanList->Assign(Value);
	}

	StringGridClear(sgServer);

	for (int i = 0; i < ServerVanList->Count; i++) {
		SetServerVan(-1, ServerVanList->Items[i]);
	}
}

// ---------------------------------------------------------------------------
bool TMain::LoadTrain(String TrainNum, bool WithJoin) {
	bool Result;

	String ResultMessage;

	ShowWaitCursor();

	StartLoad();

	ServerVanList = NULL;

	ProcMess();

	TDBOracleLoadTrain * DBOracleLoadTrain =
		new TDBOracleLoadTrain(Main->Settings->ServerOracleConnection,
		TrainNum, WithJoin);
	try {
		Result = DBOracleLoadTrain->Execute();

		ResultMessage = DBOracleLoadTrain->ErrorMessage;

		ServerVanList = DBOracleLoadTrain->VanList;
	}
	__finally {
		DBOracleLoadTrain->Free();

		EndLoad();

		RestoreCursor();
	}

	if (Result) {
		if (ServerVanList->Count == 0) {
			MsgBox(Format(IDS_ERROR_RWNUM_NOT_EXISTS, TrainNum));
		}
	}
	else {
		MsgBoxErr(Format(IDS_ERROR_TRAIN_LOAD, ResultMessage));
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
