// ---------------------------------------------------------------------------

#ifndef NVansTDBLocalLoadVansH
#define NVansTDBLocalLoadVansH

#include "NVansTVan.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBLocalLoadVans : public TDatabaseOperation {
private:
	TDate FDate;

	TVanList * FVanList;

public:
	__fastcall TDBLocalLoadVans(TConnectionInfo * ConnectionInfo, TDate Date);
	__fastcall ~TDBLocalLoadVans();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property TDate Date = {read = FDate};

	__property TVanList * VanList = {read = FVanList};
};

// ---------------------------------------------------------------------------
#endif
