// ---------------------------------------------------------------------------

#ifndef NVansServerTrainsH
#define NVansServerTrainsH

// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>

#include "NVansColumns.h"

#include "NVansTOracleTrain.h"
#include "NVansTFilterOracleTrains.h"

// ---------------------------------------------------------------------------
class TfrmServerTrains : public TForm {
__published:
	TStatusBar *StatusBar;
	TStringGrid *sgList;
	TPanel *PanelTop;
	TButton *btnUpdate;
	TButton *btnClose;
	TPanel *PanelFilter;
	TDateTimePicker *pckrFilterDate;
	TLabeledEdit *eFilterVanNum;
	TLabeledEdit *eFilterInvoiceNum_1;
	TLabel *lblFilterDate;
	TButton *btnClear;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall sgListDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State);
	void __fastcall btnUpdateClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall sgListDblClick(TObject *Sender);
	void __fastcall sgListSelectCell(TObject *Sender, int ACol, int ARow,
		bool &CanSelect);
	void __fastcall sgListKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift);
	void __fastcall pckrFilterDateKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift);
	void __fastcall btnClearClick(TObject *Sender);
	void __fastcall eFilterVanNumChange(TObject *Sender);

private:
	int SelectedRow;

	TNVansServerTrainsColumns * Columns;

	TFilterOracleTrains * Filter;

	TOracleTrainList * FTrainList;

	// -----------------------------------------------------------------------
	void SetControlsEnabled(const bool Enabled);

	int SetTrain(int Index, TOracleTrain * Train);

	void SetTrainList(TOracleTrainList * Value);

	void UpdateFilter();

	bool LoadTrains();
	void UpdateTrains();

public:
	__fastcall TfrmServerTrains(TComponent* Owner);

	// -----------------------------------------------------------------------
	void StartLoad();
	void EndLoad();

	// -----------------------------------------------------------------------
	__property TOracleTrainList * TrainList = {
		read = FTrainList, write = SetTrainList};
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmServerTrains *frmServerTrains;
// ---------------------------------------------------------------------------
#endif
