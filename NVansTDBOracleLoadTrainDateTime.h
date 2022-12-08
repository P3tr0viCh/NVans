// ---------------------------------------------------------------------------

#ifndef NVansTDBOracleLoadTrainDateTimeH
#define NVansTDBOracleLoadTrainDateTimeH

#include "NVansTOracleVan.h"

#include "NVansTKeyOracleTrain.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBOracleLoadTrainDateTime : public TDatabaseOperation {
private:
	TKeyOracleTrain * FKeyOracleTrain;

	TKeyOracleTrainList * FKeyOracleTrainList;

public:
	__fastcall TDBOracleLoadTrainDateTime(TConnectionInfo * ConnectionInfo,
		TKeyOracleTrain * KeyOracleTrain);
	__fastcall ~TDBOracleLoadTrainDateTime();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property TKeyOracleTrain * KeyOracleTrain = {read = FKeyOracleTrain};

	__property TKeyOracleTrainList * KeyOracleTrainList = {
		read = FKeyOracleTrainList};
};

// ---------------------------------------------------------------------------
#endif
