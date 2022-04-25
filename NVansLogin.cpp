// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsFileIni.h>

#include "NVansMain.h"
#include "NVansStrings.h"

#include "NVansLogin.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TfrmLogin::TfrmLogin(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
bool TfrmLogin::Show(String RightPass) {
	bool Result = false;

	TfrmLogin * frmLogin = new TfrmLogin(Application);
	try {
		frmLogin->RightPass = RightPass;

		Result = frmLogin->ShowModal() == mrOk;
	}
	__finally {
		delete frmLogin;
	}

	WriteToLog(Result ? IDS_LOG_LOGIN_OK : IDS_LOG_LOGIN_CANCEL);

	return Result;
}

// ---------------------------------------------------------------------------
bool TfrmLogin::CheckPass() {
	if (!AnsiSameStr(ePass->Text, RightPass)) {
		WriteToLog(IDS_LOG_ERROR_PASS_WRONG);

		ePass->Clear();
		ePass->SetFocus();

		MsgBoxErr(IDS_ERROR_PASS_WRONG);

		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLogin::FormCreate(TObject *Sender) {
	TFileIni * FileIni = TFileIni::GetNewInstance();
	try {
		FileIni->ReadFormPosition(this);
	}
	__finally {
		delete FileIni;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLogin::FormDestroy(TObject *Sender) {
	TFileIni * FileIni = TFileIni::GetNewInstance();
	try {
		FileIni->WriteFormPosition(this);
	}
	__finally {
		delete FileIni;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLogin::btnOKClick(TObject *Sender) {
	if (!CheckPass()) {
		ModalResult = mrNone;
	}
}

// ---------------------------------------------------------------------------
