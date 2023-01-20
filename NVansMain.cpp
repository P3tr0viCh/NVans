// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Clipbrd.hpp"

#include <AboutFrm.h>

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsSQL.h>
#include <UtilsMisc.h>
#include <UtilsKAndM.h>
#include <UtilsFiles.h>
#include <UtilsFileIni.h>
#include <UtilsStringGrid.h>

#include <DBOperation.h>
#include <DBOperationEvent.h>
#include <DBOperationCheck.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsGridHeader.h"

#include "NVansFindMatch.h"

#include "NVansDBOperationTags.h"

#include "NVansDBOracleLoadTrain.h"
#include "NVansDBOracleLoadTrainDateTime.h"
#include "NVansDBOracleLoadTrains.h"

#include "NVansDBLocalLoadTrains.h"
#include "NVansDBLocalLoadVans.h"
#include "NVansDBLocalSaveVan.h"
#include "NVansDBLocalSaveVanProps.h"

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

	DefaultRowHeight = Canvas->TextHeight("ComboBox") + 8;

	ServerColumns = new TServerColumns();
	LocalColumns = new TLocalColumns();

	ServerOptions = new TStringGridOptions(sgServer);
	ServerOptions->ColSizing = true; // TODO
	ServerOptions->ColorChanged = Main->Settings->ColorChanged;
	ServerOptions->ColorReadOnly = Main->Settings->ColorReadOnly;
	ServerOptions->ColorSelected = Main->Settings->ColorSelected;
	ServerOptions->DefaultRowHeight = Main->DefaultRowHeight;

	LocalOptions = new TStringGridOptions(sgLocal);
	LocalOptions->ColSizing = true; // TODO
	LocalOptions->ColorChanged = Main->Settings->ColorChanged;
	LocalOptions->ColorReadOnly = Main->Settings->ColorReadOnly;
	LocalOptions->ColorSelected = Main->Settings->ColorSelected;
	LocalOptions->DefaultRowHeight = Main->DefaultRowHeight;

	FKeyOracleTrain = new TKeyOracleTrain();

	FServerVanList = new TOracleVanList();
	FLocalVanList = new TLocalVanList();

	Caption = Application->Title + " " + GetFileVer(Application->ExeName);

	StringGridInit(sgServer, ServerColumns);
	StringGridInit(sgLocal, LocalColumns);

	TFileIni * FileIni = TFileIni::GetNewInstance();
	try {
		FileIni->ReadFormBounds(this);

		StringGridColWidthsReadFromIni(sgServer, FileIni, Name,
			"ServerColWidths");
		StringGridColWidthsReadFromIni(sgLocal, FileIni, Name,
			"LocalColWidths");
	}
	__finally {
		delete FileIni;
	}

	if (!Settings->Load()) {
		MsgBoxErr(IDS_ERROR_LOAD_SETTINGS);

		Application->Terminate();

		return;
	}

	SettingsChanged();

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

	FKeyOracleTrain->Free();

	LocalOptions->Free();
	ServerOptions->Free();

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
	switch (Settings->ScaleType) {
	case stAvitekDyn:
	case stAvitekSta:
	case stWME:
		break;
	default:
		return;
	}

	KeyOracleTrain->TrainNum = "42";
	KeyOracleTrainChanged();

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
	StringGridDrawCell(sgServer, ACol, ARow, Rect, State, ServerColumns,
		ServerOptions);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::sgLocalDrawCell(TObject *Sender, int ACol, int ARow,
	TRect &Rect, TGridDrawState State) {
	// TODO
	// StringGridDrawCell(sgLocal, ACol, ARow, Rect, State, IsLocalVanTare(ARow) ?
	// LocalColumns->ReadOnlyIfTare : TIntegerSet(), LocalColumns->LeftAlign,
	// TIntegerSet(), Main->Settings->ColorReadOnly, clMax, true, false,
	// Main->Settings->ColorChanged, Main->Settings->ColorSelected);
	StringGridDrawCell(sgLocal, ACol, ARow, Rect, State, LocalColumns,
		LocalOptions);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::eRWNumKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift) {
	if (Key == VK_RETURN && Shift.Empty()) {
		btnServerLoad->Click();
	}
}

// ---------------------------------------------------------------------------
void TMain::MenuItemAction(TMenuItemAction Action) {
	if (ActiveControl == NULL) {
		return;
	}

	if (!ActiveControl->ClassNameIs("TStringGrid")) {
		return;
	}

	TStringGrid * S = (TStringGrid*) ActiveControl;

	if (StringGridIsEmpty(S)) {
		return;
	}

	String Text;

	String S2;

	TGridRect Selection;

	TLocalVan * LocalVan;

	TStringList * SL;

	switch (Action) {
	case maCopy:
		for (int R = S->Selection.Top; R <= S->Selection.Bottom; R++) {
			for (int C = S->Selection.Left; C <= S->Selection.Right - 1; C++) {
				Text = Text + S->Cells[C][R] + TAB;
			}
			Text = Text + S->Cells[S->Selection.Right][R];
			Text = Text + sLineBreak;
		}

		Clipboard()->AsText = Text;

		break;
	case maPaste:
		SL = new TStringList();
		try {
			Text = Clipboard()->AsText;

			while (!Text.IsEmpty()) {
				SplitStr(Text, sLineBreak, 0, S2, Text);
				SL->Add(S2);
			}

			if (SL->Count == 0) {
				return;
			}

			for (int i = 0; i < SL->Count; i++) {
				if (SL->Strings[i].IsEmpty()) {
					continue;
				}
				if (SL->Strings[i].Length() > 8 || !IsInt(SL->Strings[i])) {
					return;
				}
			}

			for (int LocalIndex = sgLocal->Row, i = 0;
			LocalIndex < sgLocal->RowCount && i < SL->Count; LocalIndex++, i++)
			{
				if (!sgLocal->Cells[TLocalColumns::VANNUM][LocalIndex]
					.IsEmpty()) {
					if (!MsgBoxYesNo(IDS_QUESTION_DATA_OVERWRITE)) {
						return;
					}
					break;
				}
			}

			for (int LocalIndex = sgLocal->Row, i = 0;
			LocalIndex < sgLocal->RowCount && i < SL->Count; LocalIndex++, i++)
			{
				LocalVan = LocalVanList->Items[LocalIndex - 1];

				LocalVan->VanNum = SL->Strings[i];

				SetLocalVan(LocalIndex, LocalVan);

				StringGridRowSetChanged(sgLocal, LocalIndex, true);
			}

			LocalChanged = true;
		}
		__finally {
			SL->Free();
		}
		break;
	case maClear:
		for (int R = S->Selection.Top; R <= S->Selection.Bottom; R++) {
			LocalVan = LocalVanList->Items[R - 1];

			LocalVan->CalcFields = false;

			for (int C = S->Selection.Left; C <= S->Selection.Right; C++) {
				switch (C) {
				case TLocalColumns::VANNUM:
					LocalVan->VanNum = "";
					break;
				case TLocalColumns::CARGOTYPE:
					LocalVan->CargoType = "";
					break;
				case TLocalColumns::INVOICE_NUM:
					LocalVan->InvoiceNum = "";
					break;
				case TLocalColumns::INVOICE_SUPPLIER:
					LocalVan->InvoiceSupplier = "";
					break;
				case TLocalColumns::INVOICE_RECIPIENT:
					LocalVan->InvoiceRecipient = "";
					break;
				case TLocalColumns::DEPART_STATION:
					LocalVan->DepartStation = "";
					break;
				case TLocalColumns::PURPOSE_STATION:
					LocalVan->PurposeStation = "";
					break;
				case TLocalColumns::CARRYING:
					LocalVan->Carrying = 0;
					break;
				case TLocalColumns::TARE_T:
					LocalVan->TareT = 0;
					break;
				case TLocalColumns::INVOICE_NETTO:
					LocalVan->InvoiceNetto = 0;
					break;
				case TLocalColumns::INVOICE_TARE:
					LocalVan->InvoiceTare = 0;
					break;
				}
			}

			LocalVan->CalcFields = true;

			SetLocalVan(R, LocalVan);

			StringGridRowSetChanged(sgLocal, R, true);
		}

		LocalChanged = true;

		break;
	case maSelectAll:
		StringGridInvalidateSelected(S);

		Selection.Left = 1;
		Selection.Right = S->ColCount - 1;

		Selection.Top = 1;
		Selection.Bottom = S->RowCount - 1;

		S->Selection = Selection;

		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::miCopyClick(TObject *Sender) {
	MenuItemAction(maCopy);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::miPasteVanNumClick(TObject *Sender) {
	MenuItemAction(maPaste);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::miSelectAllClick(TObject *Sender) {
	MenuItemAction(maSelectAll);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::miClearClick(TObject *Sender) {
	MenuItemAction(maClear);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::PopupMenuPopup(TObject *Sender) {
	if (ActiveControl == sgLocal) {
		miCopy->Enabled = LocalVanList->Count > 0;

		miPasteVanNum->Visible = true;
		miPasteVanNum->Enabled = miCopy->Enabled;
	}
	else {
		miCopy->Enabled = ServerVanList->Count > 0;

		miPasteVanNum->Visible = false;
		miPasteVanNum->Enabled = false;
	}

	miClear->Visible = miPasteVanNum->Visible;
	miClear->Enabled = miPasteVanNum->Visible && LocalVanList->Count > 0;

	miSelectAll->Enabled = miCopy->Enabled;
}

// ---------------------------------------------------------------------------
void TMain::SetControlsEnabled(const bool Enabled) {
	PanelServer->Enabled = Enabled;
	sgServer->Enabled = Enabled;
	PanelCommon->Enabled = Enabled;
	sgLocal->Enabled = Enabled;
	PanelLocal->Enabled = Enabled;
}

// ---------------------------------------------------------------------------
void TMain::UpdateScaleType() {
	PanelCommon->Visible = Settings->ScaleType != stDisabled;

	sgLocal->Visible = Settings->ScaleType == stAvitekDyn ||
		Settings->ScaleType == stAvitekSta;

	btnSaveVanProps->Visible = sgLocal->Visible;

	PanelLocal->Visible = sgLocal->Visible;

	btnLocalTrains->Visible = Settings->ScaleType == stAvitekDyn;

	if (sgLocal->Visible) {
		sgServer->Align = alTop;
		PanelCommon->Align = alTop;
		sgServer->Height =
			(ClientHeight - PanelServer->Height - PanelCommon->Height -
			PanelLocal->Height - StatusBar->Height) / 2;
	}
	else {
		sgServer->Align = alClient;
		PanelCommon->Align = alBottom;
	}
}

// ---------------------------------------------------------------------------
void TMain::SettingsChanged() {
	eRWNum->Text = "";

	ServerVanList = NULL;

	LocalVanList = NULL;

	LocalChanged = false;

	UpdateScaleType();

	EndOperation();
}

// ---------------------------------------------------------------------------
void TMain::StartOperation(TOperation Operation) {
	ShowWaitCursor();

	NativeUInt Ident = 0;

	switch (Operation) {
	case oLoad:
		Ident = IDS_STATUS_DATA_LOAD;
		break;
	case oSave:
		Ident = IDS_STATUS_DATA_SAVE;
		break;
	}

	StatusBar->SimpleText = LoadStr(Ident);

	SetControlsEnabled(false);

	frmServerTrains->StartLoad();
	frmLocalTrains->StartLoad();

	ProcMess();
}

// ---------------------------------------------------------------------------
void TMain::EndOperation() {
	if (frmLocalTrains) {
		frmLocalTrains->EndLoad();
	}
	if (frmServerTrains) {
		frmServerTrains->EndLoad();
	}

	SetControlsEnabled(true);

	StatusBar->SimpleText = "";

	btnServerReverse->Enabled = ServerVanList->Count > 1;
	btnServerSaveToFile->Enabled = btnServerReverse->Enabled;

	switch (Settings->ScaleType) {
	case stAvitekDyn:
	case stAvitekSta:
		btnSaveVanProps->Enabled = ServerVanList->Count > 0;

		btnCopyDataAll->Enabled =
			btnSaveVanProps->Enabled && LocalVanList->Count > 0;
		btnCopyDataMass->Enabled = btnCopyDataAll->Enabled;

		break;
	case stWME:
		btnCopyDataAll->Enabled = ServerVanList->Count > 0;
		btnCopyDataMass->Enabled = btnCopyDataAll->Enabled;

		break;
	}

	RestoreCursor();
}

// ---------------------------------------------------------------------------
int TMain::SetServerVan(int Index, TOracleVan * Van) {
	if (Index < 0) {
		Index = StringGridAddRow(sgServer);
	}

	sgServer->Objects[TServerColumns::VAN_OBJECT][Index] = Van;

	sgServer->Cells[TServerColumns::NUM][Index] = IntToStr(Index);

	sgServer->Cells[TServerColumns::VANNUM][Index] = Van->VanNum;

	sgServer->Cells[TServerColumns::CARGOTYPE][Index] = Van->CargoType;

	sgServer->Cells[TServerColumns::INVOICE_NUM][Index] = Van->InvoiceNum;

	sgServer->Cells[TServerColumns::INVOICE_SUPPLIER][Index] =
		Van->InvoiceSupplier;
	sgServer->Cells[TServerColumns::INVOICE_RECIPIENT][Index] =
		Van->InvoiceRecipient;

	sgServer->Cells[TServerColumns::DEPART_STATION][Index] = Van->DepartStation;
	sgServer->Cells[TServerColumns::PURPOSE_STATION][Index] =
		Van->PurposeStation;

	StringGridSetCellInt(sgServer, TServerColumns::CARRYING, Index,
		Van->Carrying);
	StringGridSetCellInt(sgServer, TServerColumns::TARE_T, Index, Van->TareT);

	StringGridSetCellInt(sgServer, TServerColumns::INVOICE_NETTO, Index,
		Van->InvoiceNetto);
	StringGridSetCellInt(sgServer, TServerColumns::INVOICE_TARE, Index,
		Van->InvoiceTare);

	return Index;
}

// ---------------------------------------------------------------------------
int TMain::SetLocalVan(int Index, TLocalVan * Van) {
	if (Index < 0) {
		Index = StringGridAddRow(sgLocal);
	}

	sgLocal->Objects[TLocalColumns::VAN_OBJECT][Index] = Van;

	sgLocal->Cells[TLocalColumns::NUM][Index] = IntToStr(Index);

	sgLocal->Cells[TLocalColumns::DATETIME][Index] = DTToS(Van->DateTime);

	sgLocal->Cells[TLocalColumns::VANNUM][Index] = Van->VanNum;

	sgLocal->Cells[TLocalColumns::CARGOTYPE][Index] = Van->CargoType;

	sgLocal->Cells[TLocalColumns::INVOICE_NUM][Index] = Van->InvoiceNum;

	sgLocal->Cells[TLocalColumns::INVOICE_SUPPLIER][Index] =
		Van->InvoiceSupplier;
	sgLocal->Cells[TLocalColumns::INVOICE_RECIPIENT][Index] =
		Van->InvoiceRecipient;

	sgLocal->Cells[TLocalColumns::DEPART_STATION][Index] = Van->DepartStation;
	sgLocal->Cells[TLocalColumns::PURPOSE_STATION][Index] = Van->PurposeStation;

	StringGridSetCellInt(sgLocal, TLocalColumns::CARRYING, Index,
		Van->Carrying);
	StringGridSetCellInt(sgLocal, TLocalColumns::TARE_T, Index, Van->TareT);
	StringGridSetCellInt(sgLocal, TLocalColumns::INVOICE_NETTO, Index,
		Van->InvoiceNetto);
	StringGridSetCellInt(sgLocal, TLocalColumns::INVOICE_TARE, Index,
		Van->InvoiceTare);

	StringGridSetCellInt(sgLocal, TLocalColumns::NETTO_DIFF, Index,
		Van->NettoDiff);

	return Index;
}

// ---------------------------------------------------------------------------
bool IsRightTrainNum(String Value) {
	return IsInt(Value);
}

// ---------------------------------------------------------------------------
void TMain::SetKeyOracleTrain(TKeyOracleTrain * Value) {
	KeyOracleTrain->Assign(Value);
	KeyOracleTrainChanged();
}

// ---------------------------------------------------------------------------
void TMain::KeyOracleTrainChanged() {
	eRWNum->Tag = true;
	eRWNum->Text = KeyOracleTrain->TrainNum;
	eRWNum->Tag = false;

	if (KeyOracleTrain->DateTime == DEFAULT_DATETIME) {
		ServerLoadTrainDateTime(KeyOracleTrain);

		if (KeyOracleTrain->DateTime == DEFAULT_DATETIME) {
			eDateTime->Text = "";
			return;
		}
	}

	eDateTime->Text = DTToS(KeyOracleTrain->DateTime, false);

	bool WithJoin = IsRightTrainNum(KeyOracleTrain->TrainNum) && !IsShift();

	ServerLoadTrain(WithJoin);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::eRWNumChange(TObject *Sender) {
	if (eRWNum->Tag) {
		return;
	}

	KeyOracleTrain->DateTime = DEFAULT_DATETIME;
	eDateTime->Text = "";
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

	StringGridClear(sgServer);

	ProcMess();

	for (int i = 0; i < ServerVanList->Count; i++) {
		SetServerVan(-1, ServerVanList->Items[i]);
	}
}

// ---------------------------------------------------------------------------
void TMain::SetLocalVanList(TLocalVanList * Value) {
	if (Value == NULL) {
		LocalVanList->Clear();
	}
	else {
		LocalVanList->Assign(Value);
	}

	StringGridClear(sgLocal);

	ProcMess();

	for (int i = 0; i < LocalVanList->Count; i++) {
		SetLocalVan(-1, LocalVanList->Items[i]);
	}
}

// ---------------------------------------------------------------------------
void TMain::ServerLoadTrain(bool WithJoin) {
	StartOperation(oLoad);

	ServerVanList = NULL;

	TDBOracleLoadTrain * DBOracleLoadTrain =
		new TDBOracleLoadTrain(Main->Settings->ServerOracleConnection, this,
		KeyOracleTrain, WithJoin);
	try {
		DBOracleLoadTrain->Tag = DB_OPERATION_ORACLE_LOAD_TRAIN;

		DBOracleLoadTrain->SQLToLog = Settings->SQLToLog;

		DBOracleLoadTrain->Execute();

		ServerVanList = DBOracleLoadTrain->VanList;
	}
	__finally {
		DBOracleLoadTrain->Free();

		EndOperation();
	}
}

// ---------------------------------------------------------------------------
void TMain::ServerLoadTrainDateTime(TKeyOracleTrain * KeyOracleTrain) {
	StartOperation(oLoad);

	TDBOracleLoadTrainDateTime * DBOracleLoadTrainDateTime =
		new TDBOracleLoadTrainDateTime(Main->Settings->ServerOracleConnection,
		this, KeyOracleTrain);
	try {
		DBOracleLoadTrainDateTime->Tag =
			DB_OPERATION_ORACLE_LOAD_TRAIN_DATETIME;

		DBOracleLoadTrainDateTime->SQLToLog = Settings->SQLToLog;

		DBOracleLoadTrainDateTime->Execute();
	}
	__finally {
		DBOracleLoadTrainDateTime->Free();

		EndOperation();
	}
}

// ---------------------------------------------------------------------------
void TMain::LocalLoadVans() {
	StartOperation(oLoad);

	LocalVanList = NULL;

	LocalChanged = false;

	TDBLocalLoadVans * DBLocalLoadVans =
		new TDBLocalLoadVans(Main->Settings->LocalConnection, this, DateLocal,
		LocalTrainNum);
	try {
		DBLocalLoadVans->Tag = DB_OPERATION_LOCAL_LOAD_VANS;

		DBLocalLoadVans->SQLToLog = Settings->SQLToLog;

		DBLocalLoadVans->Execute();

		LocalVanList = DBLocalLoadVans->VanList;
	}
	__finally {
		DBLocalLoadVans->Free();

		EndOperation();
	}
}

// ---------------------------------------------------------------------------
bool TMain::LocalSaveVans() {
	bool Result = false;

	int SaveCount = 0;

	StartOperation(oSave);

	TDBLocalSaveVan * DBLocalSaveVan =
		new TDBLocalSaveVan(Main->Settings->LocalConnection, this);
	try {
		DBLocalSaveVan->Tag = DB_OPERATION_LOCAL_SAVE_VAN;

		DBLocalSaveVan->SQLToLog = Settings->SQLToLog;

		for (int i = 1; i < sgLocal->RowCount; i++) {
			if (!StringGridRowIsChanged(sgLocal, i)) {
				continue;
			}

			DBLocalSaveVan->Van = LocalVanList->Items[i - 1];

			Result = DBLocalSaveVan->Execute();

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

		EndOperation();

		if (SaveCount > 0) {
			WriteToLog(Format(IDS_LOG_LOCAL_SAVE_VANS_OK,
				ARRAYOFCONST((SaveCount))));
		}
	}

	return Result;
}

// ---------------------------------------------------------------------------
void TMain::LocalSaveVanProps() {
	StartOperation(oSave);

	TDBLocalSaveVanProps * DBLocalSaveVanProps =
		new TDBLocalSaveVanProps(Main->Settings->LocalConnection, this,
		ServerVanList);
	try {
		DBLocalSaveVanProps->Tag = DB_OPERATION_LOCAL_SAVE_VAN_PROPS;

		DBLocalSaveVanProps->SQLToLog = Settings->SQLToLog;

		DBLocalSaveVanProps->Execute();
	}
	__finally {
		DBLocalSaveVanProps->Free();

		EndOperation();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnServerLoadClick(TObject *Sender) {
	if (eRWNum->Text.IsEmpty()) {
		eRWNum->SetFocus();
		MsgBoxErr(IDS_ERROR_NEED_RWNUM);
		return;
	}

	KeyOracleTrain->TrainNum = eRWNum->Text;
	KeyOracleTrainChanged();
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnLocalLoadClick(TObject *Sender) {
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
				SettingsChanged();
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormResize(TObject *Sender) {
	if (!Visible) {
		return;
	}

	if (!sgLocal->Visible) {
		return;
	}

	TimerResize->Enabled = false;
	TimerResize->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::TimerResizeTimer(TObject *Sender) {
	TimerResize->Enabled = false;

	if (sgLocal->Visible) {
		sgServer->Height =
			(ClientHeight - PanelServer->Height - PanelCommon->Height -
			PanelLocal->Height - StatusBar->Height) / 2;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnServerTrainsClick(TObject * Sender) {
	frmServerTrains->Visible = !frmServerTrains->Visible;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnLocalTrainsClick(TObject * Sender) {
	frmLocalTrains->Visible = !frmLocalTrains->Visible;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnSaveVanPropsClick(TObject * Sender) {
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
	switch (Settings->ScaleType) {
	case stAvitekDyn:
	case stAvitekSta:
		if (AvitekCheckNeedSave()) {
			return;
		}

		CopyData(((TButton*) Sender)->Tag == 0);

		break;
	case stWME:
		SendDataToWME(((TButton*) Sender)->Tag == 0);

		break;
	}
}

// ---------------------------------------------------------------------------
bool TMain::CheckField(int LocalColumn, int ServerColumn, int LocalIndex,
	int ServerIndex) {
	if (!sgLocal->Cells[LocalColumn][LocalIndex].IsEmpty()) {
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

		if (CheckField(TLocalColumns::CARGOTYPE, TServerColumns::CARGOTYPE,
			LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(TLocalColumns::INVOICE_NUM, TServerColumns::INVOICE_NUM,
			LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(TLocalColumns::INVOICE_SUPPLIER,
			TServerColumns::INVOICE_SUPPLIER, LocalIndex, ServerIndex)) {
			return true;
		}
		if (CheckField(TLocalColumns::INVOICE_RECIPIENT,
			TServerColumns::INVOICE_RECIPIENT, LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(TLocalColumns::DEPART_STATION,
			TServerColumns::DEPART_STATION, LocalIndex, ServerIndex)) {
			return true;
		}
		if (CheckField(TLocalColumns::PURPOSE_STATION,
			TServerColumns::PURPOSE_STATION, LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(TLocalColumns::CARRYING, TServerColumns::CARRYING,
			LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(TLocalColumns::TARE_T, TServerColumns::TARE_T,
			LocalIndex, ServerIndex)) {
			return true;
		}

		if (CheckField(TLocalColumns::INVOICE_NETTO,
			TServerColumns::INVOICE_NETTO, LocalIndex, ServerIndex)) {
			return true;
		}
		if (CheckField(TLocalColumns::INVOICE_TARE,
			TServerColumns::INVOICE_TARE, LocalIndex, ServerIndex)) {
			return true;
		}
	}

	return false;
}

// ---------------------------------------------------------------------------
void TMain::CopyData(bool CopyAll) {
	TStringList * Source = new TStringList();
	TStringList * Dest = new TStringList();

	for (int i = 0; i < ServerVanList->Count; i++) {
		Source->Add(ServerVanList->Items[i]->VanNum);
	}

	for (int i = 0; i < LocalVanList->Count; i++) {
		if (LocalVanList->Items[i]->IsLoaded) {
			Dest->Add(LocalVanList->Items[i]->VanNum);
		}
		else {
			Dest->Add("*");
		}
	}

	TIntegerPairList * Result = new TIntegerPairList();

	try {
		FindMatch(Source, Dest, Result);

		if (Result->Count == 0) {
			WriteToLog(IDS_LOG_COPY_DATA_NOT_FOUND);
			MsgBox(IDS_MSG_MATCH_NOT_FOUND);
			return;
		}

		TGridRect Selection;

		Selection.Left = TLocalColumns::VANNUM;
		Selection.Right = TLocalColumns::VANNUM;

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
		Selection.Top = MinInt2 + 1;
		if (MaxInt2 - MinInt2 > Result->Count) {
			Selection.Bottom = Selection.Top;
		}
		else {
			Selection.Bottom = MaxInt2 + 1;
		}

		sgLocal->Row = Selection.Top;
		sgLocal->Selection = Selection;

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

			ServerVan = ServerVanList->Items[ServerIndex];
			LocalVan = LocalVanList->Items[LocalIndex];

			LocalVan->CalcFields = false;

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

			LocalVan->TareT = ServerVan->TareT;

			if (LocalVan->TareIndex == tiTrafaret) {
				LocalVan->Tare = LocalVan->TareT;
				LocalVan->TareScaleNum = LocalVan->ScaleNum;
				LocalVan->TareDateTime = Now();
			}

			LocalVan->InvoiceNetto = ServerVan->InvoiceNetto;
			LocalVan->InvoiceTare = ServerVan->InvoiceTare;

			LocalVan->CalcFields = true;

			SetLocalVan(LocalIndex + 1, LocalVan);

			StringGridRowSetChanged(sgLocal, LocalIndex + 1, true);
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
void __fastcall TMain::sgServerSelectCell(TObject * Sender, int ACol, int ARow,
	bool &CanSelect) {
	StringGridInvalidateSelected((TStringGrid*) Sender);
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
void __fastcall TMain::sgLocalDblClick(TObject * Sender) {
#ifdef _DEBUG
	if (sgLocal->Row > 0) {
		TLocalVan * Van = LocalVanList->Items[sgLocal->Row - 1];
		MsgBox(StringReplace(Van->ToString(), ",", sLineBreak,
			TReplaceFlags() << rfReplaceAll));
	}
#endif
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnServerReverseClick(TObject * Sender) {
	TOracleVanList * VanList = new TOracleVanList();
	try {
		TOracleVan * Van;

		for (int i = ServerVanList->Count - 1; i >= 0; i--) {
			Van = new TOracleVan();
			Van->Assign(ServerVanList->Items[i]);

			VanList->Add(Van);
		}

		ServerVanList = VanList;
	}
	__finally {
		VanList->Free();
	}
}

// ---------------------------------------------------------------------------
void TMain::DBOperationEventStart(TObject * Sender) {
	String Log;

	switch (((TDBOperation*)Sender)->Tag) {
	case DB_OPERATION_CHECK:
		Log = Format(IDS_LOG_DATABASE_CONNECT,
			ARRAYOFCONST((((TDBConnectionServer*)((TDBOperationCheck*) Sender)
			->DBConnection)->User,
			((TDBConnectionServer*)((TDBOperationCheck*)Sender)->DBConnection)
			->Host, ((TDBConnectionServer*)((TDBOperationCheck*)Sender)
			->DBConnection)->Port,
			((TDBConnectionServer*)((TDBOperationCheck*)Sender)->DBConnection)
			->Database)));
		break;
	case DB_OPERATION_ORACLE_LOAD_TRAIN:
	case DB_OPERATION_ORACLE_LOAD_TRAIN_DATETIME:
	case DB_OPERATION_ORACLE_LOAD_TRAINS:
	case DB_OPERATION_LOCAL_LOAD_TRAINS:
	case DB_OPERATION_LOCAL_LOAD_VANS:
	case DB_OPERATION_LOCAL_SAVE_VAN:
	case DB_OPERATION_LOCAL_SAVE_VAN_PROPS:
		break;
	default:
		throw Exception("unknown dboperation start: " + Sender->ClassName());
	}

	if (!Log.IsEmpty()) {
		WriteToLog(Log);
	}
}

// ---------------------------------------------------------------------------
void TMain::DBOperationEventEndOK(TObject * Sender) {
	String Log;
	String Message;

	switch (((TDBOperation*)Sender)->Tag) {
	case DB_OPERATION_CHECK:
		Log = Format(IDS_LOG_DATABASE_CONNECT_OK,
			((TDBOperationCheck*)Sender)->DBVersion);

		Message = Format(IDS_MSG_DATABASE_CONNECT_OK,
			((TDBOperationCheck*)Sender)->DBVersion);

		break;
	case DB_OPERATION_ORACLE_LOAD_TRAIN:
		Log = Format(IDS_LOG_ORACLE_LOAD_TRAIN_OK,
			ARRAYOFCONST((((TDBOracleLoadTrain*)Sender)->VanList->Count)));

		if (((TDBOracleLoadTrain*)Sender)->VanList->Count == 0) {
			Message = Format(IDS_MSG_RWNUM_NOT_EXISTS,
			KeyOracleTrain->TrainNum);
		}

		break;
	case DB_OPERATION_ORACLE_LOAD_TRAIN_DATETIME:
		Log = Format(IDS_LOG_ORACLE_LOAD_TRAIN_DATETIME_OK,
			ARRAYOFCONST((((TDBOracleLoadTrainDateTime*)Sender)
			->KeyOracleTrainList->Count)));

		if (((TDBOracleLoadTrainDateTime*)Sender)
			->KeyOracleTrainList->Count != 0) {
			KeyOracleTrain->DateTime = ((TDBOracleLoadTrainDateTime*)Sender)
				->KeyOracleTrainList->Items[0]->DateTime;
		}
		else {
			KeyOracleTrain->DateTime = DEFAULT_DATETIME;
		}

		if (KeyOracleTrain->DateTime == DEFAULT_DATETIME) {
			Message = Format(IDS_MSG_RWNUM_NOT_EXISTS,
			KeyOracleTrain->TrainNum);
		}

		break;
	case DB_OPERATION_ORACLE_LOAD_TRAINS:
		Log = Format(IDS_LOG_ORACLE_LOAD_TRAINS_OK,
			ARRAYOFCONST((((TDBOracleLoadTrains*)Sender)->TrainList->Count)));

		break;
	case DB_OPERATION_LOCAL_LOAD_TRAINS:
		Log = Format(IDS_LOG_LOCAL_LOAD_TRAINS_OK,
			ARRAYOFCONST((((TDBLocalLoadTrains*)Sender)->TrainList->Count)));

		break;
	case DB_OPERATION_LOCAL_LOAD_VANS:
		Log = Format(IDS_LOG_LOCAL_LOAD_VANS_OK,
			ARRAYOFCONST((((TDBLocalLoadVans*)Sender)->VanList->Count)));

		break;
	case DB_OPERATION_LOCAL_SAVE_VAN:
		break;
	case DB_OPERATION_LOCAL_SAVE_VAN_PROPS:
		Log = Format(IDS_LOG_LOCAL_SAVE_VAN_PROPS_OK,
			ARRAYOFCONST((((TDBLocalSaveVanProps*)Sender)->InsertCount,
			((TDBLocalSaveVanProps*)Sender)->UpdateCount)));

		break;
	default:
		throw Exception("unknown dboperation end ok: " + Sender->ClassName());
	}

	if (!Log.IsEmpty()) {
		WriteToLog(Log);
	}

	if (!Message.IsEmpty()) {
		MsgBox(Message);
	};
}

// ---------------------------------------------------------------------------
void TMain::DBOperationEventEndFail(TObject * Sender) {
	NativeUInt LogId;
	NativeUInt MessageId;

	switch (((TDBOperation*)Sender)->Tag) {
	case DB_OPERATION_CHECK:
		LogId = IDS_LOG_DATABASE_CONNECT_FAIL;
		MessageId = IDS_MSG_DATABASE_CONNECT_FAIL;

		break;
	case DB_OPERATION_ORACLE_LOAD_TRAIN:
		LogId = IDS_LOG_ORACLE_LOAD_TRAIN_FAIL;
		MessageId = IDS_ERROR_ORACLE_LOAD_TRAINS;

		break;
	case DB_OPERATION_ORACLE_LOAD_TRAIN_DATETIME:
		LogId = IDS_LOG_ORACLE_LOAD_TRAIN_DATETIME_FAIL;
		MessageId = IDS_ERROR_ORACLE_LOAD_TRAIN_DATETIME;

		break;
	case DB_OPERATION_ORACLE_LOAD_TRAINS:
		LogId = IDS_LOG_ORACLE_LOAD_TRAINS_FAIL;
		MessageId = IDS_ERROR_ORACLE_LOAD_TRAINS;

		break;
	case DB_OPERATION_LOCAL_LOAD_TRAINS:
		LogId = IDS_LOG_LOCAL_LOAD_TRAINS_FAIL;
		MessageId = IDS_ERROR_LOCAL_LOAD_TRAINS;

		break;
	case DB_OPERATION_LOCAL_LOAD_VANS:
		LogId = IDS_LOG_LOCAL_LOAD_VANS_FAIL;
		MessageId = IDS_ERROR_LOCAL_LOAD_VANS;

		break;
	case DB_OPERATION_LOCAL_SAVE_VAN:
		LogId = IDS_LOG_LOCAL_SAVE_VANS_FAIL;
		MessageId = IDS_ERROR_LOCAL_SAVE_VANS;

		break;
	case DB_OPERATION_LOCAL_SAVE_VAN_PROPS:
		LogId = IDS_LOG_LOCAL_SAVE_VAN_PROPS_FAIL;
		MessageId = IDS_ERROR_LOCAL_SAVE_VAN_PROPS;

		break;
	default:
		throw Exception("unknown dboperation end fail: " + Sender->ClassName());
	}

	WriteToLog(Format(LogId, ((TDBOperation*)Sender)->ErrorMessage));

	MsgBoxErr(Format(MessageId, ((TDBOperation*)Sender)->ErrorMessage));
}

String CsvStr(String S) {
	if (S.IsEmpty()) {
		return S;
	}

	for (int i = S.Length(); i > 1; i--) {
		if (S[i] == '"') {
			S.Insert('"', i);
		}
	}

	S = String('"') + S + String('"');

	return S;
}

// ---------------------------------------------------------------------------
void TMain::ServerSaveTrainToFile(TOracleVanList * ServerVanList,
	String FileName) {
	StartOperation(oSave);

	TStringList * List = new TStringList();
	try {
		String S;

		for (int i = 0; i < ServerVanList->Count; i++) {
			S = DateTimeToStr(ServerVanList->Items[i]->InvoiceDateTime);
			S += ";";
			S += CsvStr(ServerVanList->Items[i]->VanNum);
			S += ";";
			S += CsvStr(ServerVanList->Items[i]->CargoType);
			S += ";";
			S += CsvStr(ServerVanList->Items[i]->InvoiceNum);
			S += ";";
			S += CsvStr(ServerVanList->Items[i]->InvoiceSupplier);
			S += ";";
			S += CsvStr(ServerVanList->Items[i]->InvoiceRecipient);
			S += ";";
			S += CsvStr(ServerVanList->Items[i]->DepartStation);
			S += ";";
			S += CsvStr(ServerVanList->Items[i]->PurposeStation);
			S += ";";
			S += IntToStr(ServerVanList->Items[i]->Carrying);
			S += ";";
			S += IntToStr(ServerVanList->Items[i]->TareT);
			S += ";";
			S += IntToStr(ServerVanList->Items[i]->InvoiceNetto);
			S += ";";
			S += IntToStr(ServerVanList->Items[i]->InvoiceTare);

			List->Add(S);
		}

		List->SaveToFile(FileName);
	}
	__finally {
		List->Free();

		EndOperation();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMain::btnServerSaveToFileClick(TObject *Sender) {
	SaveDialog->FileName = "";

	if (SaveDialog->Execute()) {
		ServerSaveTrainToFile(ServerVanList, SaveDialog->FileName);
	}
}

// ---------------------------------------------------------------------------
void TMain::SendDataToWME(bool SendAll) {
	MsgBox(BoolToStr(SendAll, true));
}

// ---------------------------------------------------------------------------
