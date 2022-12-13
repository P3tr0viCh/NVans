// ---------------------------------------------------------------------------

#ifndef NVansDBOracleLoadTrainH
#define NVansDBOracleLoadTrainH

#include <DBOperation.h>

#include "NVansOracleVan.h"

#include "NVansKeyOracleTrain.h"

// ---------------------------------------------------------------------------
class TDBOracleLoadTrain : public TDBOperation {
private:
	TKeyOracleTrain * FKeyOracleTrain;

	bool FWithJoin;

	TOracleVanList * FVanList;

public:
	__fastcall TDBOracleLoadTrain(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent, TKeyOracleTrain * KeyOracleTrain,
		bool WithJoin);
	__fastcall ~TDBOracleLoadTrain();

	void Operation();

	// -----------------------------------------------------------------------
	__property TKeyOracleTrain * KeyOracleTrain = {read = FKeyOracleTrain};

	__property bool WithJoin = {read = FWithJoin};

	__property TOracleVanList * VanList = {read = FVanList};
};

// ---------------------------------------------------------------------------
#endif
