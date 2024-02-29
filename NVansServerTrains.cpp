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

#include "NVansAdd.h"

#include "NVansColumns.h"
#include "NVansStrings.h"
#include "NVansStringsGridHeader.h"

#include "NVansDBOperationTags.h"

#include "NVansDBOracleLoadTrains.h"

#include "NVansMain.h"

#include "NVansServerTrains.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmServerTrains *frmServerTrains;

// ---------------------------------------------------------------------------
__fastcall TfrmServerTrains::TfrmServerTrains(TComponent* Owner) : TForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::FormCreate(TObject *Sender) {
	SelectedRow = -1;

	ListColumns = new TServerTrainsColumns();

	ListOptions = new TStringGridOptions(sgList);
	ListOptions->ColSizing = true; // TODO

	Filter = new TFilterOracleTrains();

	FTrainList = new TOracleTrainList();

	StringGridInit(sgList, ListColumns);

	TFileIni * FileIni = TFileIni::GetNewInstance();
	try {
		FileIni->ReadFormBounds(this);

		StringGridColWidthsReadFromIni(sgList, FileIni, Name, "ColWidths");
	}
	__finally {
		delete FileIni;
	}

#ifdef _DEBUG
	pckrFilterDate->Date = StrToDate("12.04.2022");
#else
	pckrFilterDate->Date = Now();
#endif
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::FormShow(TObject *Sender) {
	UpdateTrains();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::FormDestroy(TObject *Sender) {
	TFileIni * FileIni = TFileIni::GetNewInstance();
	try {
		FileIni->WriteFormBounds(this);

		StringGridColWidthsWriteToIni(sgList, FileIni, Name, "ColWidths");
	}
	__finally {
		delete FileIni;
	}

	FTrainList->Free();
	Filter->Free();

	ListOptions->Free();
	ListColumns->Free();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::sgListDrawCell(TObject *Sender, int ACol,
	int ARow, TRect &Rect, TGridDrawState State) {
	StringGridDrawCell(sgList, ACol, ARow, Rect, State, ListOptions);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::btnUpdateClick(TObject *Sender) {
	UpdateTrains();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::btnCloseClick(TObject *Sender) {
	Close();
}

// ---------------------------------------------------------------------------
void TfrmServerTrains::SetControlsEnabled(const bool Enabled) {
	btnUpdate->Enabled = Enabled;
	btnClear->Enabled = Enabled;

	sgList->Enabled = Enabled;

	PanelFilter->Enabled = Enabled;
}

// ---------------------------------------------------------------------------
void TfrmServerTrains::StartLoad() {
	SetControlsEnabled(false);
}

// ---------------------------------------------------------------------------
void TfrmServerTrains::EndLoad() {
	SetControlsEnabled(true);
}

// ---------------------------------------------------------------------------
int TfrmServerTrains::SetTrain(int Index, TOracleTrain * Train) {
	if (Index < 0) {
		Index = StringGridAddRow(sgList);
	}

	sgList->Cells[TServerTrainsColumns::RWNUM][Index] = Train->TrainNum;
	sgList->Cells[TServerTrainsColumns::DATETIME][Index] =
		DTToS(Train->DateTime, false);
	sgList->Cells[TServerTrainsColumns::VAN_COUNT][Index] =
		IntToStr(Train->VanCount);

	return Index;
}

// ---------------------------------------------------------------------------
void TfrmServerTrains::SetTrainList(TOracleTrainList * Value) {
	if (Value == NULL) {
		TrainList->Clear();
	}
	else {
		TrainList->Assign(Value);
	}

	SelectedRow = -1;

	StringGridClear(sgList);

	ProcMess();

	for (int i = 0; i < TrainList->Count; i++) {
		SetTrain(-1, TrainList->Items[i]);
	}

	SelectedRow = StringGridIsEmpty(sgList) ? -1 : 1;
}

// ---------------------------------------------------------------------------
void TfrmServerTrains::LoadTrains() {
	ShowWaitCursor();

	Main->StartOperation(Main->oLoad);

	TrainList = NULL;

	ProcMess();

	TDBOracleLoadTrains * DBOracleLoadTrains =
		new TDBOracleLoadTrains(Main->Settings->OracleConnection, Main,
		Filter);
	try {
		DBOracleLoadTrains->Tag = DB_OPERATION_ORACLE_LOAD_TRAINS;

		DBOracleLoadTrains->SQLToLog = Main->Settings->SQLToLog;

		DBOracleLoadTrains->Execute();

		TrainList = DBOracleLoadTrains->TrainList;
	}
	__finally {
		DBOracleLoadTrains->Free();

		Main->EndOperation();

		RestoreCursor();
	}
}

// ---------------------------------------------------------------------------
void TfrmServerTrains::UpdateFilter() {
	Filter->Date = pckrFilterDate->Date;
	Filter->VanNum = NormalizeVanNumSQL(eFilterVanNum->Text);
	Filter->InvoiceNum_1 = eFilterInvoiceNum_1->Text;
}

// ---------------------------------------------------------------------------
void TfrmServerTrains::UpdateTrains() {
	SelectedRow = -1;

	StatusBar->Panels->Items[0]->Text = "";

	UpdateFilter();

	LoadTrains();

	StatusBar->Panels->Items[0]->Text =
		Format(IDS_STATUS_TRAIN_LIST_COUNT, IntToStr(TrainList->Count));
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::sgListDblClick(TObject *Sender) {
	if (SelectedRow < 0) {
		return;
	}

	if (StringGridIsEmpty(sgList)) {
		return;
	}

	Main->KeyOracleTrain->TrainNum =
		TrainList->Items[SelectedRow - 1]->TrainNum;
	Main->KeyOracleTrain->DateTime =
		TrainList->Items[SelectedRow - 1]->DateTime;
	Main->UseAutoReplace = !IsShift();
	Main->KeyOracleTrainChanged();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::sgListSelectCell(TObject *Sender, int ACol,
	int ARow, bool &CanSelect) {
	StringGridInvalidateSelected(sgList);

	if (StringGridIsEmpty(sgList)) {
		return;
	}

	if (SelectedRow == ARow) {
		return;
	}

	SelectedRow = ARow;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::sgListKeyDown(TObject *Sender, WORD &Key,
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
void __fastcall TfrmServerTrains::pckrFilterDateKeyDown(TObject *Sender,
	WORD &Key, TShiftState Shift) {
	if (Key == VK_RETURN && Shift.Empty()) {
		btnUpdate->Click();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::btnClearClick(TObject *Sender) {
#ifdef _DEBUG
	pckrFilterDate->Date = StrToDate("12.04.2022");
#else
	pckrFilterDate->Date = Now();
#endif

	eFilterVanNum->Text = "";

	eFilterInvoiceNum_1->Text = "";

	btnUpdate->Click();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::eFilterVanNumChange(TObject *Sender) {
	pckrFilterDate->Enabled = eFilterVanNum->Text.IsEmpty()
		&& eFilterInvoiceNum_1->Text.IsEmpty();
}

// ---------------------------------------------------------------------------
