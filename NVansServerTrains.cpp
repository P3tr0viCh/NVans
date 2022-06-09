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

#include "NVansTDBOracleLoadTrains.h"

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

	Columns = new TNVansServerTrainsColumns();

	Filter = new TFilterOracleTrains();

	FTrainList = new TOracleTrainList();

	StringGridInit(sgList, Columns, Main->DefaultRowHeight);

	sgList->Options = sgList->Options << goColSizing;

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
	Columns->Free();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::sgListDrawCell(TObject *Sender, int ACol,
	int ARow, TRect &Rect, TGridDrawState State) {
	StringGridDrawCell(sgList, ACol, ARow, Rect, State, NUSet,
		Columns->LeftAlign, NUSet, Main->Settings->ColorReadOnly, NUColor, true,
		false, NUColor, false, NUColor);
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

	sgList->Cells[TNVansServerTrainsColumns::RWNUM][Index] = Train->TrainNum;
	sgList->Cells[TNVansServerTrainsColumns::DATETIME][Index] =
		DTToS(Train->DateTime, false);
	sgList->Cells[TNVansServerTrainsColumns::VAN_COUNT][Index] =
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
bool TfrmServerTrains::LoadTrains() {
	bool Result;

	String ResultMessage;

	ShowWaitCursor();

	Main->StartDBOperation(Main->dboLoad);

	TrainList = NULL;

	ProcMess();

	TDBOracleLoadTrains * DBOracleLoadTrains =
		new TDBOracleLoadTrains(Main->Settings->ServerOracleConnection, Filter);
	try {
		Result = DBOracleLoadTrains->Execute();

		ResultMessage = DBOracleLoadTrains->ErrorMessage;

		TrainList = DBOracleLoadTrains->TrainList;
	}
	__finally {
		DBOracleLoadTrains->Free();

		Main->EndDBOperation();

		RestoreCursor();
	}

	if (!Result) {
		MsgBoxErr(Format(IDS_ERROR_ORACLE_TRAINS_LOAD, ResultMessage));
	}

	return Result;
}

// ---------------------------------------------------------------------------
void TfrmServerTrains::UpdateFilter() {
	Filter->Date = pckrFilterDate->Date;
	Filter->VanNum = eFilterVanNum->Text;
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

	Main->OracleTrainNum = TrainList->Items[SelectedRow - 1]->TrainNum;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::sgListSelectCell(TObject *Sender, int ACol,
	int ARow, bool &CanSelect) {
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
void __fastcall TfrmServerTrains::btnFilterClearClick(TObject *Sender) {
#ifdef _DEBUG
	pckrFilterDate->Date = StrToDate("12.04.2022");
#else
	pckrFilterDate->Date = Now();
#endif

	eFilterVanNum->Text = "";

	btnUpdate->Click();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerTrains::eFilterVanNumChange(TObject *Sender) {
	pckrFilterDate->Enabled = IsEmpty(eFilterVanNum->Text);
}
// ---------------------------------------------------------------------------
