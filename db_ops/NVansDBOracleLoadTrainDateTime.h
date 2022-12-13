// ---------------------------------------------------------------------------

#ifndef NVansDBOracleLoadTrainDateTimeH
#define NVansDBOracleLoadTrainDateTimeH

#include <DBOperation.h>

#include "NVansOracleVan.h"

#include "NVansKeyOracleTrain.h"

// ---------------------------------------------------------------------------
class TDBOracleLoadTrainDateTime : public TDBOperation {
private:
	TKeyOracleTrain * FKeyOracleTrain;

	TKeyOracleTrainList * FKeyOracleTrainList;

public:
	__fastcall TDBOracleLoadTrainDateTime(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent, TKeyOracleTrain * KeyOracleTrain);
	__fastcall ~TDBOracleLoadTrainDateTime();

	void Operation();

	// -----------------------------------------------------------------------
	__property TKeyOracleTrain * KeyOracleTrain = {read = FKeyOracleTrain};

	__property TKeyOracleTrainList * KeyOracleTrainList = {
		read = FKeyOracleTrainList};
};

// ---------------------------------------------------------------------------
#endif
