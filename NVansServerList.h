// ---------------------------------------------------------------------------

#ifndef NVansServerListH
#define NVansServerListH

// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>

#include "NVansTOracleTrain.h"

// ---------------------------------------------------------------------------
class TfrmServerList : public TForm {
__published:
	TStatusBar *StatusBar;
	TStringGrid *sgList;
	TPanel *PanelTop;
	TButton *btnServerLoad;
	TButton *btnClose;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall sgListDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State);
	void __fastcall btnServerLoadClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall sgListDblClick(TObject *Sender);
	void __fastcall sgListSelectCell(TObject *Sender, int ACol, int ARow,
		bool &CanSelect);

private:
	TDate FDate;

	int SelectedRow;

	TOracleTrainList * FTrainList;

	// -----------------------------------------------------------------------
	void CreateColumns();

	void SetControlsEnabled(const bool Enabled);

	int SetTrain(int Index, TOracleTrain * Train);

	void SetDate(TDate Value);
	void SetTrainList(TOracleTrainList * Value);

	bool LoadTrains();

public:
	__fastcall TfrmServerList(TComponent* Owner);

	// -----------------------------------------------------------------------
	__property TDate Date = {read = FDate, write = SetDate};
	__property TOracleTrainList * TrainList = {
		read = FTrainList, write = SetTrainList};
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmServerList *frmServerList;
// ---------------------------------------------------------------------------
#endif
