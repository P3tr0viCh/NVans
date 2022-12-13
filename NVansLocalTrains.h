// ---------------------------------------------------------------------------

#ifndef NVansLocalTrainsH
#define NVansLocalTrainsH

// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>

#include "NVansColumns.h"

#include "NVansLocalTrain.h"
#include "NVansFilterLocalTrains.h"

// ---------------------------------------------------------------------------
class TfrmLocalTrains : public TForm {
__published:
	TPanel *PanelTop;
	TButton *btnUpdate;
	TButton *btnClose;
	TStatusBar *StatusBar;
	TStringGrid *sgList;

	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall sgListDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall sgListDblClick(TObject *Sender);
	void __fastcall btnUpdateClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall sgListSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);


private:
	int SelectedRow;

	TNVansLocalTrainsColumns * Columns;

	TFilterLocalTrains * Filter;

	TLocalTrainList * FTrainList;

	// -----------------------------------------------------------------------
	void SetControlsEnabled(const bool Enabled);

	int SetTrain(int Index, TLocalTrain * Train);

	void SetTrainList(TLocalTrainList * Value);

	void LoadTrains();

	void UpdateTrains();

public:
	__fastcall TfrmLocalTrains(TComponent* Owner);

	// -----------------------------------------------------------------------
	void StartLoad();
	void EndLoad();

	// -----------------------------------------------------------------------
	__property TLocalTrainList * TrainList = {
		read = FTrainList, write = SetTrainList};
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmLocalTrains *frmLocalTrains;
// ---------------------------------------------------------------------------
#endif
