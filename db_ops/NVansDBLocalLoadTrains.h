// ---------------------------------------------------------------------------

#ifndef NVansDBLocalLoadTrainsH
#define NVansDBLocalLoadTrainsH

#include <DBOperation.h>

#include "NVansLocalTrain.h"
#include "NVansFilterLocalTrains.h"

// ---------------------------------------------------------------------------
class TDBLocalLoadTrains : public TDBOperation {
private:
	TFilterLocalTrains * FFilter;

	TLocalTrainList * FTrainList;

public:
	__fastcall TDBLocalLoadTrains(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent, TFilterLocalTrains * Filter);
	__fastcall ~TDBLocalLoadTrains();

	void Operation();

	// -----------------------------------------------------------------------
	__property TFilterLocalTrains * Filter = {read = FFilter};

	__property TLocalTrainList * TrainList = {read = FTrainList};
};

// ---------------------------------------------------------------------------
#endif
