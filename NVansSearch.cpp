// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <UtilsMisc.h>
#include <UtilsFileIni.h>

#include "NVansMain.h"

#include "NVansSearch.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmSearch *frmSearch;

// ---------------------------------------------------------------------------
__fastcall TfrmSearch::TfrmSearch(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSearch::FormCreate(TObject *Sender) {
	TFileIni * FileIni = TFileIni::GetNewInstance();
	try {
		FileIni->ReadFormPosition(this);
	}
	__finally {
		delete FileIni;
	}

	ClientHeight = eSearch->Height;
	ClientWidth = eSearch->Width;
	Constraints->MaxHeight = Height;
	Constraints->MinHeight = Height;
	Constraints->MinWidth = Width;

	StartSearchEvent = Main->StartSearchEvent;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSearch::FormDestroy(TObject *Sender) {
	TFileIni * FileIni = TFileIni::GetNewInstance();
	try {
		FileIni->WriteFormPosition(this);
	}
	__finally {
		delete FileIni;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSearch::eSearchKeyPress(TObject *Sender,
	System::WideChar &Key) {
	if (Key == VK_ESCAPE) {
		Hide();
		return;
	}

	if (Key == VK_RETURN) {
		DoStartSearchEvent();
		return;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSearch::FormActivate(TObject *Sender) {
	eSearch->SelectAll();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSearch::FormHide(TObject *Sender) {
	eSearch->Clear();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSearch::eSearchChange(TObject *Sender) {
	if (Visible) {
		Timer->Enabled = false;
		Timer->Enabled = true;
	}
}

// ---------------------------------------------------------------------------
void TfrmSearch::DoStartSearchEvent() {
	if (FStartSearchEvent == NULL) {
		return;
	}

	eSearch->Enabled = false;

	String Text = eSearch->Text;

	bool Found = false;

	FStartSearchEvent(this, Text, Found);

	if (Text.IsEmpty() || Found) {
		eSearch->Color = clWindow;
		eSearch->Font->Color = clWindowText;
	}
	else {
		eSearch->Color = clRed;
		eSearch->Font->Color = clWhite;
	}

	eSearch->Enabled = true;

	if (Visible) {
		eSearch->SetFocus();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSearch::TimerTimer(TObject *Sender) {
	Timer->Enabled = false;

	DoStartSearchEvent();
}

// ---------------------------------------------------------------------------
