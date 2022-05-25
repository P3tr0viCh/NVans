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

#include "NVansServerList.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmServerList *frmServerList;

static TNVansServerListColumns Columns;

// ---------------------------------------------------------------------------
__fastcall TfrmServerList::TfrmServerList(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void TfrmServerList::CreateColumns() {
	sgList->ColCount = Columns.VISIBLE_COUNT;

	StringGridSetHeader(sgList, Columns.RWNUM, IDS_GRID_HEADER_RWNUM, 80);
	StringGridSetHeader(sgList, Columns.DATETIME,
		IDS_GRID_HEADER_DATETIME, 112);
	StringGridSetHeader(sgList, Columns.VAN_COUNT,
		IDS_GRID_HEADER_VAN_COUNT, 56);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerList::FormCreate(TObject *Sender) {
	SelectedRow = -1;

	Filter = new TFilterOracleTrains();

	FTrainList = new TOracleTrainList();

	CreateColumns();

	sgList->DefaultRowHeight = Main->DefaultRowHeight;

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
void __fastcall TfrmServerList::FormShow(TObject *Sender) {
	UpdateTrains();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerList::FormDestroy(TObject *Sender) {
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
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerList::sgListDrawCell(TObject *Sender, int ACol,
	int ARow, TRect &Rect, TGridDrawState State) {
	StringGridDrawCell(sgList, ACol, ARow, Rect, State, NUSet,
		Columns.LeftAlign, NUSet, Main->Settings->ColorReadOnly, NUColor, true,
		false, false, NUColor, false, NUColor);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerList::btnServerLoadClick(TObject *Sender) {
	UpdateTrains();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerList::btnCloseClick(TObject *Sender) {
	Close();
}

// ---------------------------------------------------------------------------
void TfrmServerList::SetControlsEnabled(const bool Enabled) {
	btnServerLoad->Enabled = Enabled;

	sgList->Enabled = Enabled;

	PanelFilter->Enabled = Enabled;
}

// ---------------------------------------------------------------------------
void TfrmServerList::StartLoad() {
	SetControlsEnabled(false);
}

// ---------------------------------------------------------------------------
void TfrmServerList::EndLoad() {
	SetControlsEnabled(true);
}

// ---------------------------------------------------------------------------
int TfrmServerList::SetTrain(int Index, TOracleTrain * Train) {
	if (Index < 0) {
		if (!StringGridIsEmpty(sgList)) {
			sgList->RowCount++;
		}
		Index = sgList->RowCount - 1;
	}

	sgList->Cells[Columns.RWNUM][Index] = Train->TrainNum;
	sgList->Cells[Columns.DATETIME][Index] = DTToS(Train->DateTime, false);
	sgList->Cells[Columns.VAN_COUNT][Index] = IntToStr(Train->VanCount);

	return Index;
}

// ---------------------------------------------------------------------------
void TfrmServerList::SetTrainList(TOracleTrainList * Value) {
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
bool TfrmServerList::LoadTrains() {
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
void TfrmServerList::UpdateFilter() {
	Filter->Date = pckrFilterDate->Date;
	Filter->VanNum = eFilterVanNum->Text;
}

// ---------------------------------------------------------------------------
void TfrmServerList::UpdateTrains() {
	SelectedRow = -1;

	StatusBar->Panels->Items[0]->Text = "";

	UpdateFilter();

	LoadTrains();

	StatusBar->Panels->Items[0]->Text = Format(IDS_STATUS_TRAIN_LIST_VAN_COUNT,
		IntToStr(TrainList->Count));
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerList::sgListDblClick(TObject *Sender) {
	if (SelectedRow < 0) {
		return;
	}

	if (StringGridIsEmpty(sgList)) {
		return;
	}

	Main->TrainNum = TrainList->Items[SelectedRow - 1]->TrainNum;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerList::sgListSelectCell(TObject *Sender, int ACol,
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
void __fastcall TfrmServerList::sgListKeyDown(TObject *Sender, WORD &Key,
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
void __fastcall TfrmServerList::pckrFilterDateKeyDown(TObject *Sender,
	WORD &Key, TShiftState Shift) {
	if (Key == VK_RETURN && Shift.Empty()) {
		btnServerLoad->Click();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerList::btnFilterClearClick(TObject *Sender) {
#ifdef _DEBUG
	pckrFilterDate->Date = StrToDate("12.04.2022");
#else
	pckrFilterDate->Date = Now();
#endif

	eFilterVanNum->Text = "";

	btnServerLoad->Click();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmServerList::eFilterVanNumChange(TObject *Sender) {
	pckrFilterDate->Enabled = IsEmpty(eFilterVanNum->Text);
}
// ---------------------------------------------------------------------------
