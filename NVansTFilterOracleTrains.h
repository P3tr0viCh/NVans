// ---------------------------------------------------------------------------

#ifndef NVansTFilterOracleTrainsH
#define NVansTFilterOracleTrainsH

#include <System.Classes.hpp>

// ---------------------------------------------------------------------------
class TFilterOracleTrains : public TObject {
private:
	TDate FDate;

	String FVanNum;

	String FInvoiceNum_1;

	// -----------------------------------------------------------------------
	void Init();

public:
	__fastcall TFilterOracleTrains();
	__fastcall ~TFilterOracleTrains();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TFilterOracleTrains * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property TDate Date = {read = FDate, write = FDate};

	__property String VanNum = {read = FVanNum, write = FVanNum};

	__property String InvoiceNum_1 = {read = FInvoiceNum_1, write = FInvoiceNum_1};
};

// ---------------------------------------------------------------------------
#endif
