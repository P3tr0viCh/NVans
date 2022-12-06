// ---------------------------------------------------------------------------

#ifndef NVansTDBOracleLoadTrainH
#define NVansTDBOracleLoadTrainH

#include "NVansTOracleVan.h"

#include "NVansTKeyOracleTrain.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBOracleLoadTrain : public TDatabaseOperation {
private:
	TKeyOracleTrain * FKeyOracleTrain;

	bool FWithJoin;

	TOracleVanList * FVanList;

public:
	__fastcall TDBOracleLoadTrain(TConnectionInfo * ConnectionInfo,
		TKeyOracleTrain * KeyOracleTrain, bool WithJoin);
	__fastcall ~TDBOracleLoadTrain();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property TKeyOracleTrain * KeyOracleTrain = {read = FKeyOracleTrain};

	__property bool WithJoin = {read = FWithJoin};

	__property TOracleVanList * VanList = {read = FVanList};
};

// ---------------------------------------------------------------------------
#endif
