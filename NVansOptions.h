// ---------------------------------------------------------------------------

#ifndef NVansOptionsH
#define NVansOptionsH

// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

// ---------------------------------------------------------------------------
class TfrmOptions : public TForm {
__published:
	TButton *btnOK;
	TButton *btnCancel;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);

private:
public:
	__fastcall TfrmOptions(TComponent* Owner);

	static bool Show();
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmOptions *frmOptions;
// ---------------------------------------------------------------------------
#endif
