// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsFileIni.h>

#include "NVansDebug.h"

#include "NVansStrings.h"

#include "NVansMain.h"

#include "NVansTDBCheckOracle.h"

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

	TFileIni * FileIni = TFileIni::GetNewInstance();
	try {
		FileIni->ReadFormPosition(this);
	}
	__finally {
		delete FileIni;
	}

	cboxOracleDriver->Items->Add(LoadStr(IDS_CONNECTION_ORACLE_DRIVER_1));
}

// ---------------------------------------------------------------------------
void __fastcall TfrmOptions::FormDestroy(TObject *Sender) {
	TFileIni * FileIni = TFileIni::GetNewInstance();
	try {
		FileIni->WriteFormPosition(this);
	}
	__finally {
		delete FileIni;
	}

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

	eOracleHost->Text = Settings->ServerOracleConnection->Host;
	eOracleService->Text = Settings->ServerOracleConnection->Service;
	eOracleUser->Text = Settings->ServerOracleConnection->User;
	eOraclePass->Text = Settings->ServerOracleConnection->Password;
	cboxOracleDriver->ItemIndex = cboxOracleDriver->Items->IndexOf
		(Settings->ServerOracleConnection->Driver);
}

// ---------------------------------------------------------------------------
void TfrmOptions::UpdateSettings() {
	Settings->OptionsPass = eOptionsPass->Text;

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
TConnectionInfo * TfrmOptions::GetConnectionInfo(TConnectionType Type) {
	TConnectionInfo * Connection;

	switch (Type) {
	case ctServerOracle:
		Connection = new TConnectionOracle();

		Connection->User = eOracleUser->Text;
		Connection->Password = eOraclePass->Text;

		((TConnectionOracle*) Connection)->Driver = cboxOracleDriver->Text;
		((TConnectionOracle*) Connection)->Host = eOracleHost->Text;
		((TConnectionOracle*) Connection)->Service = eOracleService->Text;

		break;
	}

	return Connection;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmOptions::btnOracleCheckClick(TObject *Sender) {
	TConnectionInfo * ConnectionInfo;
	TDatabaseOperation * DatabaseOperation;

	bool Result = false;
	String ResultMessage;

	ShowWaitCursor();
	try {
		ConnectionInfo = GetConnectionInfo(ctServerOracle);

		DatabaseOperation = new TDBCheckOracle((TConnectionOracle*) ConnectionInfo);

		Result = DatabaseOperation->Execute();

		if (Result) {
			ResultMessage = Format(IDS_MSG_DATABASE_CONNECT_OK,
				((TDBCheckOracle*) DatabaseOperation)->DBVersion);
		}
		else {
			ResultMessage = Format(IDS_MSG_DATABASE_CONNECT_FAIL,
				DatabaseOperation->ErrorMessage);
		}
	}
	__finally {
		DatabaseOperation->Free();
		ConnectionInfo->Free();

		RestoreCursor();
	}

	if (Result) {
		MsgBox(ResultMessage);
	}
	else {
		MsgBoxErr(ResultMessage);
	}

}
// ---------------------------------------------------------------------------
