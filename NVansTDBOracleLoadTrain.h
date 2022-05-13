// ---------------------------------------------------------------------------

#ifndef NVansTDBOracleLoadTrainH
#define NVansTDBOracleLoadTrainH

#include "NVansTVan.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBOracleLoadTrain : public TDatabaseOperation {
private:
	String FTrainNum;

	bool FWithJoin;

	TVanList * FVanList;

public:
	__fastcall TDBOracleLoadTrain(TConnectionInfo * ConnectionInfo, String TrainNum, bool WithJoin);
	__fastcall ~TDBOracleLoadTrain();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property String TrainNum = {read = FTrainNum};

	__property bool WithJoin = {read = FWithJoin};

	__property TVanList * VanList = {read = FVanList};
};

// ---------------------------------------------------------------------------
#endif
