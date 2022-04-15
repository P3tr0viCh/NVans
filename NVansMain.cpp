// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

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
#include "NVansColumns.h"

#include "NVansLogin.h"
#include "NVansOptions.h"

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

	if (!Settings->Load()) {
		MsgBoxErr(IDS_ERROR_LOAD_SETTINGS);

		Application->Terminate();

		return;
	}

	DefaultRowHeight = Canvas->TextHeight("ComboBox") + 8;

	Caption = Application->Title + " " + GetFileVer(Application->ExeName);
	StatusBar->Panels->Items[0]->Text = LoadStr(IDS_COPYRIGHT_STATUS);

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
	eRWNum->Text = "42";
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
void TMain::SetControlsEnabled(const bool Enabled) {
	eRWNum->Enabled = Enabled;
	btnServerLoad->Enabled = Enabled;
	btnOptions->Enabled = Enabled;

	sgServer->Enabled = Enabled;
	Splitter->Enabled = Enabled;
	sgLocal->Enabled = Enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnServerLoadClick(TObject *Sender) {
	StringGridClear(sgServer);

	if (IsEmpty(eRWNum->Text)) {
		eRWNum->SetFocus();
		MsgBoxErr(IDS_ERROR_NEED_RWNUM);
		return;
	}

	ADOConnection1->ConnectionString =
		Settings->ServerOracleConnection->ConnectionString;

	ShowWaitCursor();
	SetControlsEnabled(false);

	ProcMess();

	try {
		ADOConnection1->Open();

		try {
			ADOQuery1->Connection = ADOConnection1;

			ADOQuery1->SQL->Clear();

			ADOQuery1->SQL->Add
				("SELECT INVNUM, TO_NCHAR(CARGOTYPE) CARGOTYPE, TO_NCHAR(INVOICE_NUM) INVOICE_NUM, TO_NCHAR(INVOICE_SUPPLIER) INVOICE_SUPPLIER, TO_NCHAR(INVOICE_CONSIGN) INVOICE_CONSIGN, TO_NCHAR(DEPART_STATION) DEPART_STATION, TO_NCHAR(PURPOSE_STATION) PURPOSE_STATION"
				);
			ADOQuery1->SQL->Add("FROM BPTL.RP_NVANS");
			ADOQuery1->SQL->Add("WHERE RWNUM=:RWNUM");
			ADOQuery1->SQL->Add("ORDER BY NUM");

			TParameter * Param = ADOQuery1->Parameters->ParamByName("RWNUM");
			Param->DataType = ftFixedWideChar;
			Param->Value = eRWNum->Text;

			// WriteToLog(ADOQuery1->SQL->Text);

			ADOQuery1->Open();
			try {
				if (ADOQuery1->IsEmpty()) {
					MsgBox(Format(IDS_ERROR_RWNUM_NOT_EXISTS, eRWNum->Text));
					return;
				}

				while (!ADOQuery1->Eof) {
					ProcMess();

					if (!StringGridIsEmpty(sgServer)) {
						sgServer->RowCount++;
					}

					sgServer->Cells[ServerColumns.NUM][sgServer->RowCount - 1] =
						IntToStr(sgServer->RowCount - 1);
					sgServer->Cells[ServerColumns.VANNUM][sgServer->RowCount -
						1] = ADOQuery1->FieldByName("INVNUM")->AsString;

					sgServer->Cells[ServerColumns.CARGOTYPE]
						[sgServer->RowCount - 1] =
						Trim(ADOQuery1->FieldByName("CARGOTYPE")->AsString);

					sgServer->Cells[ServerColumns.INVOICE_NUM]
						[sgServer->RowCount - 1] =
						Trim(ADOQuery1->FieldByName("INVOICE_NUM")->AsString);
					sgServer->Cells[ServerColumns.INVOICE_SUPPLIER]
						[sgServer->RowCount - 1] =
						Trim(ADOQuery1->FieldByName("INVOICE_SUPPLIER")
						->AsString);
					sgServer->Cells[ServerColumns.INVOICE_RECIPIENT]
						[sgServer->RowCount - 1] =
						Trim(ADOQuery1->FieldByName("INVOICE_CONSIGN")
						->AsString);
					sgServer->Cells[ServerColumns.DEPART_STATION]
						[sgServer->RowCount - 1] =
						Trim(ADOQuery1->FieldByName("DEPART_STATION")
						->AsString);
					sgServer->Cells[ServerColumns.PURPOSE_STATION]
						[sgServer->RowCount - 1] =
						Trim(ADOQuery1->FieldByName("PURPOSE_STATION")
						->AsString);

					ADOQuery1->Next();
				}
			}
			__finally {
				ADOQuery1->Close();
			}
		}
		__finally {
			ADOConnection1->Close();
		}
	}
	__finally {
		SetControlsEnabled(true);

		RestoreCursor();
	}
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
