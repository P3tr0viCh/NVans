// ---------------------------------------------------------------------------

#ifndef NVansOptionsH
#define NVansOptionsH

// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "NVansTSettings.h"

// ---------------------------------------------------------------------------
class TfrmOptions : public TForm {
__published:
	TPanel *PanelMain;
	TPanel *PanelButtons;
	TButton *btnOK;
	TButton *btnCancel;
	TPageControl *PageControl;
	TTabSheet *tsLocalDatabase;
	TTabSheet *tsServerOracleDatabase;
	TTabSheet *tsProgram;
	TLabeledEdit *eOracleHost;
	TLabeledEdit *eOracleService;
	TLabeledEdit *eOracleUser;
	TLabeledEdit *eOraclePass;
	TComboBox *cboxOracleDriver;
	TLabel *lblOracleLDriver;
	TButton *btnOracleCheck;
	TLabeledEdit *eLocalHost;
	TLabeledEdit *eLocalUser;
	TLabeledEdit *eLocalPass;
	TComboBox *cboxLocalDriver;
	TButton *btnLocalCheck;
	TLabel *lblLocalDriver;
	TGroupBox *gbOptionsPass;
	TLabeledEdit *eOptionsPass;
	TLabeledEdit *eOptionsPass2;
	TCheckBox *cboxUseLocal;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnOKClick(TObject *Sender);
	void __fastcall btnOracleCheckClick(TObject *Sender);

private:
	TSettings * Settings;

	// -----------------------------------------------------------------------
	void ControlSetFocus(TWinControl * Control);

	void UpdateForm();
	void UpdateSettings();

	enum TConnectionType {
		ctLocal, ctServerOracle
	};

	TConnectionInfo * GetConnectionInfo(TConnectionType Type);

public:
	__fastcall TfrmOptions(TComponent* Owner);

	static bool Show(TSettings * Settings);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmOptions *frmOptions;
// ---------------------------------------------------------------------------
#endif
