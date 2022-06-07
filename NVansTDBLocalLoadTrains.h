// ---------------------------------------------------------------------------

#ifndef NVansTDBLocalLoadTrainsH
#define NVansTDBLocalLoadTrainsH

#include "NVansTLocalTrain.h"
#include "NVansTFilterLocalTrains.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBLocalLoadTrains : public TDatabaseOperation {
private:
	TFilterLocalTrains * FFilter;

	TLocalTrainList * FTrainList;

public:
	__fastcall TDBLocalLoadTrains(TConnectionInfo * ConnectionInfo,
		TFilterLocalTrains * Filter);
	__fastcall ~TDBLocalLoadTrains();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property TFilterLocalTrains * Filter = {read = FFilter};

	__property TLocalTrainList * TrainList = {read = FTrainList};
};

// ---------------------------------------------------------------------------
#endif
