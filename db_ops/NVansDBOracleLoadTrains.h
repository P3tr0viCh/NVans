// ---------------------------------------------------------------------------

#ifndef NVansDBOracleLoadTrainsH
#define NVansDBOracleLoadTrainsH

#include <DBOperation.h>

#include "NVansOracleTrain.h"
#include "NVansFilterOracleTrains.h"

// ---------------------------------------------------------------------------
class TDBOracleLoadTrains : public TDBOperation {
private:
	TFilterOracleTrains * FFilter;

	TOracleTrainList * FTrainList;

public:
	__fastcall TDBOracleLoadTrains(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent, TFilterOracleTrains * Filter);
	__fastcall ~TDBOracleLoadTrains();

	void Operation();

	// -----------------------------------------------------------------------
	__property TFilterOracleTrains * Filter = {read = FFilter};

	__property TOracleTrainList * TrainList = {read = FTrainList};
};

// ---------------------------------------------------------------------------
#endif
