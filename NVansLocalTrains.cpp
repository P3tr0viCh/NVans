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

#include "NVansTDBLocalLoadTrains.h"

#include "NVansMain.h"

#include "NVansLocalTrains.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmLocalTrains *frmLocalTrains;

// ---------------------------------------------------------------------------
__fastcall TfrmLocalTrains::TfrmLocalTrains(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLocalTrains::FormCreate(TObject *Sender) {
	SelectedRow = -1;

	Columns = new TNVansLocalTrainsColumns();

	Filter = new TFilterLocalTrains();

	FTrainList = new TLocalTrainList();

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
	Filter->Date = StrToDate("12.02.2019");
#else
	Filter->Date = Now();
#endif
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLocalTrains::FormDestroy(TObject *Sender) {
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
void __fastcall TfrmLocalTrains::sgListDrawCell(TObject *Sender, int ACol,
	int ARow, TRect &Rect, TGridDrawState State) {
	StringGridDrawCell(sgList, ACol, ARow, Rect, State, TIntegerSet(),
		Columns->LeftAlign, TIntegerSet(), Main->Settings->ColorReadOnly, clMax,
		true, false, clMax, Main->Settings->ColorSelected);

}

// ---------------------------------------------------------------------------
void __fastcall TfrmLocalTrains::btnCloseClick(TObject *Sender) {
	Close();
}

// ---------------------------------------------------------------------------
void TfrmLocalTrains::SetControlsEnabled(const bool Enabled) {
	btnUpdate->Enabled = Enabled;

	sgList->Enabled = Enabled;
}

// ---------------------------------------------------------------------------
void TfrmLocalTrains::StartLoad() {
	SetControlsEnabled(false);
}

// ---------------------------------------------------------------------------
void TfrmLocalTrains::EndLoad() {
	SetControlsEnabled(true);
}

// ---------------------------------------------------------------------------
int TfrmLocalTrains::SetTrain(int Index, TLocalTrain * Train) {
	if (Index < 0) {
		Index = StringGridAddRow(sgList);
	}

	sgList->Cells[TNVansLocalTrainsColumns::DATETIME][Index] =
		DTToS(Train->DateTime, false);
	sgList->Cells[TNVansLocalTrainsColumns::VAN_COUNT][Index] =
		IntToStr(Train->VanCount);
	sgList->Cells[TNVansLocalTrainsColumns::TRAIN_NUM][Index] = Train->TrainNum;

	return Index;
}

// ---------------------------------------------------------------------------
void TfrmLocalTrains::SetTrainList(TLocalTrainList * Value) {
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
bool TfrmLocalTrains::LoadTrains() {
	bool Result;

	String ResultMessage;

	ShowWaitCursor();

	Main->StartDBOperation(Main->dboLoad);

	TrainList = NULL;

	ProcMess();

	TDBLocalLoadTrains * DBLocalLoadTrains =
		new TDBLocalLoadTrains(Main->Settings->LocalConnection, Filter);
	try {
		Result = DBLocalLoadTrains->Execute();

		ResultMessage = DBLocalLoadTrains->ErrorMessage;

		TrainList = DBLocalLoadTrains->TrainList;
	}
	__finally {
		DBLocalLoadTrains->Free();

		Main->EndDBOperation();

		RestoreCursor();
	}

	if (!Result) {
		MsgBoxErr(Format(IDS_ERROR_LOCAL_TRAINS_LOAD, ResultMessage));
	}

	return Result;
}

// ---------------------------------------------------------------------------
void TfrmLocalTrains::UpdateTrains() {
	String TrainNum = "";

	if (!StringGridIsEmpty(sgList) && SelectedRow > 0) {
		TrainNum = sgList->Cells[TNVansLocalTrainsColumns::TRAIN_NUM]
			[SelectedRow];
	}

	SelectedRow = -1;

	StatusBar->Panels->Items[0]->Text = "";

	LoadTrains();

	StatusBar->Panels->Items[0]->Text =
		Format(IDS_STATUS_TRAIN_LIST_COUNT, IntToStr(TrainList->Count));

	if (!TrainNum.IsEmpty()) {
		for (int i = 1; i < sgList->RowCount; i++) {
			if (AnsiSameStr(sgList->Cells[TNVansLocalTrainsColumns::TRAIN_NUM]
				[i], TrainNum)) {
				sgList->Row = i;
				break;
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLocalTrains::sgListDblClick(TObject *Sender) {
	if (SelectedRow < 0) {
		return;
	}

	if (StringGridIsEmpty(sgList)) {
		return;
	}

	Main->LocalTrainNum = sgList->Cells[TNVansLocalTrainsColumns::TRAIN_NUM]
		[SelectedRow];
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLocalTrains::btnUpdateClick(TObject *Sender) {
	UpdateTrains();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLocalTrains::FormShow(TObject *Sender) {
	UpdateTrains();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLocalTrains::sgListSelectCell(TObject *Sender, int ACol,
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
