// ---------------------------------------------------------------------------

#ifndef NVansTDBOracleLoadTrainsH
#define NVansTDBOracleLoadTrainsH

#include "NVansTOracleTrain.h"
#include "NVansTFilterOracleTrains.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBOracleLoadTrains : public TDatabaseOperation {
private:
	TFilterOracleTrains * FFilter;

	TOracleTrainList * FTrainList;

public:
	__fastcall TDBOracleLoadTrains(TConnectionInfo * ConnectionInfo,
		TFilterOracleTrains * Filter);
	__fastcall ~TDBOracleLoadTrains();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property TFilterOracleTrains * Filter = {read = FFilter};

	__property TOracleTrainList * TrainList = {read = FTrainList};
};

// ---------------------------------------------------------------------------
#endif
