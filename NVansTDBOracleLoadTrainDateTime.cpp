// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsSQL.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansTDBOracleLoadTrainDateTime.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOracleLoadTrainDateTime::TDBOracleLoadTrainDateTime
	(TConnectionInfo * ConnectionInfo, TKeyOracleTrain * KeyOracleTrain)
	: TDatabaseOperation(ConnectionInfo) {
	FKeyOracleTrainList = new TKeyOracleTrainList();

	FKeyOracleTrain = new TKeyOracleTrain();

	FKeyOracleTrain->Assign(KeyOracleTrain);
}

// ---------------------------------------------------------------------------
__fastcall TDBOracleLoadTrainDateTime::~TDBOracleLoadTrainDateTime() {
	FKeyOracleTrain->Free();
	FKeyOracleTrainList->Free();
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrainDateTime::OperationStart() {
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrainDateTime::OperationEndOK() {
	WriteToLog(Format(IDS_LOG_ORACLE_LOAD_TRAIN_DATETIME_OK,
		ARRAYOFCONST((KeyOracleTrainList->Count))));
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrainDateTime::OperationEndFail() {
	WriteToLog(Format(IDS_LOG_ORACLE_LOAD_TRAIN_DATETIME_FAIL, ErrorMessage));
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrainDateTime::Operation() {
	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		String QueryText;

		QueryText = SQLMake(QueryText, IDS_SQL_SELECT);
		QueryText = SQLMake(QueryText,
			IDS_SQL_ORACLE_NVANS_TRAIN_DATETIME_SELECT);
		QueryText = SQLMake(QueryText, IDS_SQL_FROM);
		QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TABLE);
		QueryText = SQLMake(QueryText, IDS_SQL_WHERE);
		QueryText = SQLMake(QueryText,
			IDS_SQL_ORACLE_NVANS_TRAIN_DATETIME_WHERE);
		QueryText = SQLMake(QueryText, IDS_SQL_GROUP);
		QueryText = SQLMake(QueryText,
			IDS_SQL_ORACLE_NVANS_TRAIN_DATETIME_GROUP);
		QueryText = SQLMake(QueryText, IDS_SQL_ORDER);
		QueryText = SQLMake(QueryText,
			IDS_SQL_ORACLE_NVANS_TRAIN_DATETIME_ORDER);

		Query->SQL->Text = QueryText;

#ifdef SQL_TO_LOG
		WriteToLog(Query->SQL->Text);
#endif

		SQLGetParam(Query, "RWNUM", ftFixedWideChar)->Value =
			KeyOracleTrain->TrainNum;

#ifdef SQL_TO_LOG
		WriteToLog("PARAMS: RWNUM = " + KeyOracleTrain->TrainNum);
#endif

		Query->Open();

		TKeyOracleTrain * Key;

		while (!Query->Eof) {
			ProcMess();
			if (CheckExit()) {
				throw EAbort(IDS_LOG_ERROR_TERMINATED_IN_WORK_PROGRESS);
			}

			Key = new TKeyOracleTrain();

			Key->TrainNum = KeyOracleTrain->TrainNum;

			Key->DateTime = Query->FieldByName("NDATETIME")->AsDateTime;

			FKeyOracleTrainList->Add(Key);

			Query->Next();
		}
	}
	__finally {
		Query->Free();

		Connection->Close();
	}
}

// ---------------------------------------------------------------------------
