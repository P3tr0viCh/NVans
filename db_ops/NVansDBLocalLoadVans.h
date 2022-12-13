// ---------------------------------------------------------------------------

#ifndef NVansDBLocalLoadVansH
#define NVansDBLocalLoadVansH

#include <DBOperation.h>

#include "NVansLocalVan.h"

// ---------------------------------------------------------------------------
class TDBLocalLoadVans : public TDBOperation {
private:
	TDate FDate;

	String FTrainNum;

	TLocalVanList * FVanList;

public:
	__fastcall TDBLocalLoadVans(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent, TDate Date, String TrainNum);
	__fastcall ~TDBLocalLoadVans();

	void Operation();

	// -----------------------------------------------------------------------
	__property TDate Date = {read = FDate};

	__property String TrainNum = {read = FTrainNum};

	__property TLocalVanList * VanList = {read = FVanList};
};

// ---------------------------------------------------------------------------
#endif
