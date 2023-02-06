// ---------------------------------------------------------------------------

#ifndef NVansSearchH
#define NVansSearchH

// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

typedef void __fastcall(__closure * TStartSearchEvent)(System::TObject * Sender,
	String & Text, bool &Found);

// ---------------------------------------------------------------------------
class TfrmSearch : public TForm {
__published:
	TEdit *eSearch;
	TTimer *Timer;

	void __fastcall eSearchKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall eSearchChange(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);

private:
	TStartSearchEvent FStartSearchEvent;

	// -----------------------------------------------------------------------
	void DoStartSearchEvent();

public:
	__fastcall TfrmSearch(TComponent* Owner);

	__property TStartSearchEvent StartSearchEvent = {
		read = FStartSearchEvent, write = FStartSearchEvent};
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmSearch *frmSearch;
// ---------------------------------------------------------------------------
#endif
