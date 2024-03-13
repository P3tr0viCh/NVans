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

	TOracleVanList * FVanList;

	// -----------------------------------------------------------------------
	int QueryCode(TADOQuery * Query, String FieldName);

public:
	__fastcall TDBOracleLoadTrain(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent, TKeyOracleTrain * KeyOracleTrain);
	__fastcall ~TDBOracleLoadTrain();

	void Operation();

	// -----------------------------------------------------------------------
	__property TKeyOracleTrain * KeyOracleTrain = {read = FKeyOracleTrain};

	__property TOracleVanList * VanList = {read = FVanList};
};

// ---------------------------------------------------------------------------
#endif
