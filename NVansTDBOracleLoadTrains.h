// ---------------------------------------------------------------------------

#ifndef NVansTDBOracleLoadTrainsH
#define NVansTDBOracleLoadTrainsH

#include "NVansTOracleTrain.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBOracleLoadTrains : public TDatabaseOperation {
private:
	TDate FDate;

	TOracleTrainList * FTrainList;

public:
	__fastcall TDBOracleLoadTrains(TConnectionInfo * ConnectionInfo,
		TDate Date);
	__fastcall ~TDBOracleLoadTrains();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property TDate Date = {read = FDate};

	__property TOracleTrainList * TrainList = {read = FTrainList};
};

// ---------------------------------------------------------------------------
#endif
