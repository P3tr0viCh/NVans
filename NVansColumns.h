// ---------------------------------------------------------------------------

#ifndef NVansColumnsH
#define NVansColumnsH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <UtilsStringGrid.h>

#include "NVansStringsGridHeader.h"

// ---------------------------------------------------------------------------
class TNVansServerColumns : public TStringGridBaseColumns {
public:
	static const NUM = SERVICE + 1;
	static const VANNUM = NUM + 1;
	static const CARGOTYPE = VANNUM + 1;
	static const INVOICE_NUM = CARGOTYPE + 1;
	static const INVOICE_SUPPLIER = INVOICE_NUM + 1;
	static const INVOICE_RECIPIENT = INVOICE_SUPPLIER + 1;
	static const DEPART_STATION = INVOICE_RECIPIENT + 1;
	static const PURPOSE_STATION = DEPART_STATION + 1;
	static const CARRYING = PURPOSE_STATION + 1;
	static const TARE_T = CARRYING + 1;
	static const INVOICE_NETTO = TARE_T + 1;
	static const INVOICE_TARE = INVOICE_NETTO + 1;

	static const COUNT = INVOICE_TARE + 1;

	static const VAN_OBJECT = INVOICE_TARE + 1;

	TNVansServerColumns() {
		LeftAlign =
			TIntegerSet() << VANNUM << CARGOTYPE << INVOICE_NUM <<
			INVOICE_SUPPLIER << INVOICE_RECIPIENT << DEPART_STATION <<
			PURPOSE_STATION;
	}

	// ---------------------------------------------------------------------------
	int GetCount() {
		return COUNT;
	}

	// ---------------------------------------------------------------------------
	void SetStringGridHeader(TStringGrid * Grid) {
		StringGridSetHeader(Grid, NUM, IDS_GRID_HEADER_NUM, 32);

		StringGridSetHeader(Grid, VANNUM, IDS_GRID_HEADER_VANNUM, 80);

		StringGridSetHeader(Grid, CARGOTYPE, IDS_GRID_HEADER_CARGOTYPE, 180);

		StringGridSetHeader(Grid, INVOICE_NUM,
			IDS_GRID_HEADER_INVOICE_NUM, 180);
		StringGridSetHeader(Grid, INVOICE_SUPPLIER,
			IDS_GRID_HEADER_INVOICE_SUPPLIER, 180);
		StringGridSetHeader(Grid, INVOICE_RECIPIENT,
			IDS_GRID_HEADER_INVOICE_RECIPIENT, 180);

		StringGridSetHeader(Grid, DEPART_STATION,
			IDS_GRID_HEADER_DEPART_STATION, 180);
		StringGridSetHeader(Grid, PURPOSE_STATION,
			IDS_GRID_HEADER_PURPOSE_STATION, 180);

		StringGridSetHeader(Grid, CARRYING, IDS_GRID_HEADER_CARRYING, 64);
		StringGridSetHeader(Grid, TARE_T, IDS_GRID_HEADER_TARE_T, 64);
		StringGridSetHeader(Grid, INVOICE_NETTO,
			IDS_GRID_HEADER_INVOICE_NETTO, 64);
		StringGridSetHeader(Grid, INVOICE_TARE,
			IDS_GRID_HEADER_INVOICE_TARE, 64);
	}
};

// ---------------------------------------------------------------------------
class TNVansLocalColumns : public TStringGridBaseColumns {
public:
	static const NUM = SERVICE + 1;
	static const DATETIME = NUM + 1;
	static const VANNUM = DATETIME + 1;
	static const CARGOTYPE = VANNUM + 1;
	static const INVOICE_NUM = CARGOTYPE + 1;
	static const INVOICE_SUPPLIER = INVOICE_NUM + 1;
	static const INVOICE_RECIPIENT = INVOICE_SUPPLIER + 1;
	static const DEPART_STATION = INVOICE_RECIPIENT + 1;
	static const PURPOSE_STATION = DEPART_STATION + 1;
	static const CARRYING = PURPOSE_STATION + 1;
	static const TARE_T = CARRYING + 1;
	static const INVOICE_NETTO = TARE_T + 1;
	static const INVOICE_TARE = INVOICE_NETTO + 1;
	static const NETTO_DIFF = INVOICE_TARE + 1;

	static const COUNT = NETTO_DIFF + 1;

	static const VAN_OBJECT = NETTO_DIFF + 1;

	TIntegerSet ReadOnlyIfTare;

	TNVansLocalColumns() {
		LeftAlign =
			TIntegerSet() << DATETIME << VANNUM << CARGOTYPE << INVOICE_NUM <<
			INVOICE_SUPPLIER << INVOICE_RECIPIENT << DEPART_STATION <<
			PURPOSE_STATION;

		ReadOnlyIfTare = TIntegerSet() << VANNUM;
	}

	// ---------------------------------------------------------------------------
	int GetCount() {
		return COUNT;
	}

	// ---------------------------------------------------------------------------
	void SetStringGridHeader(TStringGrid * Grid) {
		StringGridSetHeader(Grid, NUM, IDS_GRID_HEADER_NUM, 32);
		StringGridSetHeader(Grid, DATETIME, IDS_GRID_HEADER_DATETIME, 160);

		StringGridSetHeader(Grid, VANNUM, IDS_GRID_HEADER_VANNUM, 80);

		StringGridSetHeader(Grid, CARGOTYPE, IDS_GRID_HEADER_CARGOTYPE, 180);

		StringGridSetHeader(Grid, INVOICE_NUM,
			IDS_GRID_HEADER_INVOICE_NUM, 180);
		StringGridSetHeader(Grid, INVOICE_SUPPLIER,
			IDS_GRID_HEADER_INVOICE_SUPPLIER, 180);
		StringGridSetHeader(Grid, INVOICE_RECIPIENT,
			IDS_GRID_HEADER_INVOICE_RECIPIENT, 180);
		StringGridSetHeader(Grid, DEPART_STATION,
			IDS_GRID_HEADER_DEPART_STATION, 180);
		StringGridSetHeader(Grid, PURPOSE_STATION,
			IDS_GRID_HEADER_PURPOSE_STATION, 180);

		StringGridSetHeader(Grid, CARRYING, IDS_GRID_HEADER_CARRYING, 64);
		StringGridSetHeader(Grid, TARE_T, IDS_GRID_HEADER_TARE_T, 64);
		StringGridSetHeader(Grid, INVOICE_NETTO,
			IDS_GRID_HEADER_INVOICE_NETTO, 64);
		StringGridSetHeader(Grid, INVOICE_TARE,
			IDS_GRID_HEADER_INVOICE_TARE, 64);

		StringGridSetHeader(Grid, NETTO_DIFF, IDS_GRID_HEADER_NETTO_DIFF, 64);
	}
};

// ---------------------------------------------------------------------------
class TNVansServerTrainsColumns : public TStringGridBaseColumns {
public:
	static const RWNUM = SERVICE + 1;
	static const DATETIME = RWNUM + 1;
	static const VAN_COUNT = DATETIME + 1;

	static const COUNT = VAN_COUNT + 1;

	TNVansServerTrainsColumns() {
		LeftAlign = TIntegerSet() << RWNUM << DATETIME;
	}

	// ---------------------------------------------------------------------------
	int GetCount() {
		return COUNT;
	}

	// ---------------------------------------------------------------------------
	void SetStringGridHeader(TStringGrid * Grid) {
		StringGridSetHeader(Grid, RWNUM, IDS_GRID_HEADER_RWNUM, 80);
		StringGridSetHeader(Grid, DATETIME, IDS_GRID_HEADER_DATETIME, 112);
		StringGridSetHeader(Grid, VAN_COUNT, IDS_GRID_HEADER_VAN_COUNT, 56);
	}
};

// ---------------------------------------------------------------------------
class TNVansLocalTrainsColumns : public TStringGridBaseColumns {
public:
	static const DATETIME = SERVICE + 1;
	static const VAN_COUNT = DATETIME + 1;

	static const COUNT = VAN_COUNT + 1;

	static const TRAIN_NUM = VAN_COUNT + 1;

	TNVansLocalTrainsColumns() {
		LeftAlign = TIntegerSet() << DATETIME;
	}

	// ---------------------------------------------------------------------------
	int GetCount() {
		return COUNT;
	}

	// ---------------------------------------------------------------------------
	void SetStringGridHeader(TStringGrid * Grid) {
		StringGridSetHeader(Grid, DATETIME, IDS_GRID_HEADER_DATETIME, 112);
		StringGridSetHeader(Grid, VAN_COUNT, IDS_GRID_HEADER_VAN_COUNT, 56);
	}
};

// ---------------------------------------------------------------------------
#endif
