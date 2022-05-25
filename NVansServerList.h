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

#include "NVansTFilterOracleTrains.h"

#include "NVansTOracleTrain.h"

// ---------------------------------------------------------------------------
class TfrmServerList : public TForm {
__published:
	TStatusBar *StatusBar;
	TStringGrid *sgList;
	TPanel *PanelTop;
	TButton *btnServerLoad;
	TButton *btnClose;
	TPanel *PanelFilter;
	TDateTimePicker *pckrFilterDate;
	TButton *btnFilterClear;
	TLabeledEdit *eFilterVanNum;

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
	void __fastcall sgListKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift);
	void __fastcall pckrFilterDateKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift);
	void __fastcall btnFilterClearClick(TObject *Sender);
	void __fastcall eFilterVanNumChange(TObject *Sender);

private:
	int SelectedRow;

	TFilterOracleTrains * Filter;

	TOracleTrainList * FTrainList;

	// -----------------------------------------------------------------------
	void CreateColumns();

	void SetControlsEnabled(const bool Enabled);

	int SetTrain(int Index, TOracleTrain * Train);

	void SetTrainList(TOracleTrainList * Value);

	void UpdateFilter();

	bool LoadTrains();
	void UpdateTrains();

public:
	__fastcall TfrmServerList(TComponent* Owner);

	// -----------------------------------------------------------------------
	void StartLoad();
	void EndLoad();

	// -----------------------------------------------------------------------
	__property TOracleTrainList * TrainList = {
		read = FTrainList, write = SetTrainList};
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmServerList *frmServerList;
// ---------------------------------------------------------------------------
#endif
