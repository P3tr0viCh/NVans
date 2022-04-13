// ---------------------------------------------------------------------------

#ifndef NVansLoginH
#define NVansLoginH

// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TfrmLogin : public TForm {
__published:
	TButton *btnOK;
	TButton *btnCancel;
	TLabeledEdit *ePass;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);

private:
public:
	__fastcall TfrmLogin(TComponent* Owner);

	static bool Show();
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmLogin *frmLogin;
// ---------------------------------------------------------------------------
#endif
