// ---------------------------------------------------------------------------

#ifndef NVansTDBLocalLoadVansH
#define NVansTDBLocalLoadVansH

#include "NVansTLocalVan.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBLocalLoadVans : public TDatabaseOperation {
private:
	TDate FDate;

	TLocalVanList * FVanList;

public:
	__fastcall TDBLocalLoadVans(TConnectionInfo * ConnectionInfo, TDate Date);
	__fastcall ~TDBLocalLoadVans();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property TDate Date = {read = FDate};

	__property TLocalVanList * VanList = {read = FVanList};
};

// ---------------------------------------------------------------------------
#endif
