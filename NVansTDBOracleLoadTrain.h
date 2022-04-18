// ---------------------------------------------------------------------------

#ifndef NVansTDBOracleLoadTrainH
#define NVansTDBOracleLoadTrainH

#include "NVansTOracleVan.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBOracleLoadTrain : public TDatabaseOperation {
private:
	String FTrainNum;

	TOracleVanList * FVanList;

public:
	__fastcall TDBOracleLoadTrain(TConnectionInfo * ConnectionInfo, String TrainNum);
	__fastcall ~TDBOracleLoadTrain();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property String TrainNum = {read = FTrainNum};

	__property TOracleVanList * VanList = {read = FVanList};
};

// ---------------------------------------------------------------------------
#endif
