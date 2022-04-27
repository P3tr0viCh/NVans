// ---------------------------------------------------------------------------
class TNVansBaseColumns {
public:
	static const VISIBLE_COUNT = 0;

	static const COUNT = 0;

	TNVansBaseColumns() {
		ReadOnly = TIntegerSet();
		ComboBox = TIntegerSet();
		LeftAlign = TIntegerSet();
	}

	TIntegerSet ReadOnly;

	TIntegerSet ComboBox;

	TIntegerSet LeftAlign;
};

// ---------------------------------------------------------------------------
class TNVansServerColumns : public TNVansBaseColumns {
public:
	static const NUM = 0;
	static const VANNUM = 1;
	static const CARGOTYPE = 2;
	static const INVOICE_NUM = 3;
	static const INVOICE_SUPPLIER = 4;
	static const INVOICE_RECIPIENT = 5;
	static const DEPART_STATION = 6;
	static const PURPOSE_STATION = 7;
	static const CARRYING = 8;
	static const TARE_T = 9;

	static const VISIBLE_COUNT = 10;

	static const COUNT = 10;

	TNVansServerColumns() {
		LeftAlign =
			TIntegerSet() << VANNUM << CARGOTYPE << INVOICE_NUM <<
			INVOICE_SUPPLIER << INVOICE_RECIPIENT << DEPART_STATION <<
			PURPOSE_STATION;
	}
};

// ---------------------------------------------------------------------------
class TNVansLocalColumns : public TNVansBaseColumns {
public:
	static const NUM = 0;
	static const DATETIME = 1;
	static const VANNUM = 2;
	static const CARGOTYPE = 3;
	static const INVOICE_NUM = 4;
	static const INVOICE_SUPPLIER = 5;
	static const INVOICE_RECIPIENT = 6;
	static const DEPART_STATION = 7;
	static const PURPOSE_STATION = 8;

	static const VISIBLE_COUNT = 9;

	static const COUNT = 9;

	TNVansLocalColumns() {
		LeftAlign =
			TIntegerSet() << DATETIME << VANNUM << CARGOTYPE << INVOICE_NUM <<
			INVOICE_SUPPLIER << INVOICE_RECIPIENT << DEPART_STATION <<
			PURPOSE_STATION;
	}
};

// ---------------------------------------------------------------------------
class TNVansServerListColumns : public TNVansBaseColumns {
public:
	static const RWNUM = 0;
	static const DATETIME = 1;
	static const VAN_COUNT = 2;

	static const VISIBLE_COUNT = 3;

	static const COUNT = 3;

	TNVansServerListColumns() {
		LeftAlign = TIntegerSet() << RWNUM << DATETIME;
	}
};

// ---------------------------------------------------------------------------
