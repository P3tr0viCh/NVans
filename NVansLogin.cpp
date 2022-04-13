// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsFileIni.h>

#include "NVansMain.h"

#include "NVansLogin.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TfrmLogin::TfrmLogin(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
bool TfrmLogin::Show() {
	bool Result = false;

	TfrmLogin * frmLogin = new TfrmLogin(Application);
	try {
		Result = frmLogin->ShowModal() == mrOk;
	}
	__finally {
		delete frmLogin;
	}

	return Result;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLogin::FormCreate(TObject *Sender) {
	WriteToLogForm(true, ClassName());

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

	WriteToLogForm(false, ClassName());
}
// ---------------------------------------------------------------------------
