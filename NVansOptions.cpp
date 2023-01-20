// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsFileIni.h>

#include <DBConnectionMySQL.h>
#include <DBOperationCheckMySQL.h>
#include <DBOperationCheckOracle.h>

#include "NVansDebug.h"

#include "NVansStrings.h"

#include "NVansMain.h"

#include "NVansDBOperationTags.h"

#include "NVansOptions.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TfrmOptions::TfrmOptions(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
bool TfrmOptions::Show(TSettings * Settings) {
	bool Result = false;

	TfrmOptions * frmOptions = new TfrmOptions(Application);
	try {
		frmOptions->Settings->Assign(Settings);

		frmOptions->UpdateForm();

		Result = frmOptions->ShowModal() == mrOk;
	}
	__finally {
		delete frmOptions;
	}

	WriteToLog(Result ? IDS_LOG_OPTIONS_SAVE : IDS_LOG_OPTIONS_CANCEL);

	return Result;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmOptions::FormCreate(TObject *Sender) {
	Settings = new TSettings();

#ifndef _DEBUG
	PageControl->ActivePageIndex = 0;
#endif

	btnLocalCheck->Tag = ctLocal;
	btnOracleCheck->Tag = ctServerOracle;

	cboxLocalDriver->Items->Add(LoadStr(IDS_CONNECTION_MYSQL_DRIVER_0));
	cboxLocalDriver->Items->Add(LoadStr(IDS_CONNECTION_MYSQL_DRIVER_1));

	cboxOracleDriver->Items->Add(LoadStr(IDS_CONNECTION_ORACLE_DRIVER_0));
	cboxOracleDriver->Items->Add(LoadStr(IDS_CONNECTION_ORACLE_DRIVER_1));
}

// ---------------------------------------------------------------------------
void __fastcall TfrmOptions::FormDestroy(TObject *Sender) {
	Settings->Free();
}

// ---------------------------------------------------------------------------
void TfrmOptions::ControlSetFocus(TWinControl * Control) {
	TTabSheet * TabSheet;

	if (Control->Parent->ClassNameIs("TTabSheet")) {
		TabSheet = (TTabSheet*)Control->Parent;
	}
	else if (Control->Parent->Parent->ClassNameIs("TTabSheet")) {
		TabSheet = (TTabSheet*)Control->Parent->Parent;
	}

	PageControl->ActivePage = TabSheet;
	Control->SetFocus();
}

// ---------------------------------------------------------------------------
void TfrmOptions::UpdateForm() {
	eOptionsPass->Text = Settings->OptionsPass;
	eOptionsPass2->Text = Settings->OptionsPass;

	cboxSQLToLog->Checked = Settings->SQLToLog;

	rgScaleType->ItemIndex = Settings->ScaleType;

	eLocalHost->Text = Settings->LocalConnection->Host;
	eLocalUser->Text = Settings->LocalConnection->User;
	eLocalPass->Text = Settings->LocalConnection->Password;
	if (!Settings->LocalConnection->Driver.IsEmpty()) {
		cboxLocalDriver->ItemIndex =
			cboxLocalDriver->Items->IndexOf(Settings->LocalConnection->Driver);
		if (cboxLocalDriver->ItemIndex == -1) {
			cboxLocalDriver->ItemIndex =
				cboxLocalDriver->Items->Add(Settings->LocalConnection->Driver);
		}
	}

	eOracleHost->Text = Settings->ServerOracleConnection->Host;
	eOracleService->Text = Settings->ServerOracleConnection->Service;
	eOracleUser->Text = Settings->ServerOracleConnection->User;
	eOraclePass->Text = Settings->ServerOracleConnection->Password;
	if (!Settings->ServerOracleConnection->Driver.IsEmpty()) {
		cboxOracleDriver->ItemIndex =
			cboxOracleDriver->Items->IndexOf
			(Settings->ServerOracleConnection->Driver);
		if (cboxOracleDriver->ItemIndex == -1) {
			cboxOracleDriver->ItemIndex =
				cboxOracleDriver->Items->Add
				(Settings->ServerOracleConnection->Driver);
		}
	}
}

// ---------------------------------------------------------------------------
void TfrmOptions::UpdateSettings() {
	Settings->OptionsPass = eOptionsPass->Text;

	Settings->SQLToLog = cboxSQLToLog->Checked;

	Settings->ScaleType = rgScaleType->ItemIndex;

	Settings->LocalConnection->Host = eLocalHost->Text;
	Settings->LocalConnection->User = eLocalUser->Text;
	Settings->LocalConnection->Password = eLocalPass->Text;
	Settings->LocalConnection->Driver = cboxLocalDriver->Text;

	Settings->ServerOracleConnection->Host = eOracleHost->Text;
	Settings->ServerOracleConnection->Service = eOracleService->Text;
	Settings->ServerOracleConnection->User = eOracleUser->Text;
	Settings->ServerOracleConnection->Password = eOraclePass->Text;
	Settings->ServerOracleConnection->Driver = cboxOracleDriver->Text;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmOptions::btnOKClick(TObject *Sender) {
	if (!AnsiSameStr(eOptionsPass->Text, eOptionsPass2->Text)) {
		MsgBoxErr(IDS_ERROR_CHECK_PASS);

		ControlSetFocus(eOptionsPass);

		return;
	}

	UpdateSettings();

	if (Settings->Save()) {
		ModalResult = mrOk;
	}
	else {
		MsgBoxErr(IDS_ERROR_SAVE_SETTINGS);
	}
}

// ---------------------------------------------------------------------------
TDBConnection * TfrmOptions::GetConnectionInfo(TConnectionType Type) {
	TDBConnection * Connection;

	switch (Type) {
	case ctLocal:
		Connection = new TDBConnectionMySQL();

		Connection->User = eLocalUser->Text;
		Connection->Password = eLocalPass->Text;

		((TDBConnectionMySQL*) Connection)->Host = eLocalHost->Text;
		((TDBConnectionMySQL*) Connection)->Driver = cboxLocalDriver->Text;
		((TDBConnectionMySQL*) Connection)->Database =
			Settings->LocalConnection->Database;

		break;
	case ctServerOracle:
		Connection = new TDBConnectionOracle();

		Connection->User = eOracleUser->Text;
		Connection->Password = eOraclePass->Text;

		((TDBConnectionOracle*) Connection)->Host = eOracleHost->Text;
		((TDBConnectionOracle*) Connection)->Driver = cboxOracleDriver->Text;
		((TDBConnectionOracle*) Connection)->Service = eOracleService->Text;

		break;
	}

	return Connection;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmOptions::btnOracleCheckClick(TObject *Sender) {
	TDBConnection * DBConnection;
	TDBOperationCheck * DBOperationCheck;

	bool Result = false;
	String ResultMessage;

	ShowWaitCursor();
	try {
		TConnectionType ConnectionType =
			(TConnectionType)((TButton*)Sender)->Tag;

		DBConnection = GetConnectionInfo(ConnectionType);

		switch (ConnectionType) {
		case ctLocal:
			DBOperationCheck =
				new TDBOperationCheckMySQL
				((TDBConnectionMySQL*) DBConnection, Main);
			break;
		case ctServerOracle:
			DBOperationCheck =
				new TDBOperationCheckOracle
				((TDBConnectionOracle*) DBConnection, Main);
			break;
		}

		DBOperationCheck->Tag = DB_OPERATION_CHECK;

		Result = DBOperationCheck->Execute();
	}
	__finally {
		DBOperationCheck->Free();
		DBConnection->Free();

		RestoreCursor();
	}
}

// ---------------------------------------------------------------------------
