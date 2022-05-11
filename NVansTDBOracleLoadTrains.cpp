// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansTDBOracleLoadTrains.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOracleLoadTrains::TDBOracleLoadTrains
	(TConnectionInfo * ConnectionInfo, TDate Date)
	: TDatabaseOperation(ConnectionInfo) {
	FTrainList = new TOracleTrainList();

	FDate = Date;
}

// ---------------------------------------------------------------------------
__fastcall TDBOracleLoadTrains::~TDBOracleLoadTrains() {
	FTrainList->Free();
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrains::OperationStart() {
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrains::OperationEndOK() {
	WriteToLog(Format(IDS_LOG_ORACLE_LOAD_TRAINS_OK,
		ARRAYOFCONST((TrainList->Count))));
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrains::OperationEndFail() {
	WriteToLog(Format(IDS_LOG_ORACLE_LOAD_TRAINS_FAIL, ErrorMessage));
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrains::Operation() {
	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		String QueryText;

		QueryText = SQLMake(QueryText, IDS_SQL_SELECT);
		QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAINS_SELECT);
		QueryText = SQLMake(QueryText, IDS_SQL_FROM);
		QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TABLE);
		QueryText = SQLMake(QueryText, IDS_SQL_WHERE);
		QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAINS_WHERE);
		QueryText = SQLMake(QueryText, IDS_SQL_GROUP);
		QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAINS_GROUP);
		QueryText = SQLMake(QueryText, IDS_SQL_ORDER);
		QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAINS_ORDER);

		Query->SQL->Text = QueryText;

#ifdef SQL_TO_LOG
		WriteToLog(Query->SQL->Text);
#endif

		TParameter * Param1 = Query->Parameters->ParamByName("DATE_FROM");
		Param1->DataType = ftDate;
		Param1->Value = Date;
		TParameter * Param2 = Query->Parameters->ParamByName("DATE_TO");
		Param2->DataType = ftDate;
		Param2->Value = Date + 1;

#ifdef SQL_TO_LOG
		WriteToLog("PARAMS: DATE_FROM = " + VarToStr(Param1->Value) + ", " +
			"DATE_TO = " + VarToStr(Param2->Value));
#endif

		Query->Open();

		TOracleTrain * Train;

		while (!Query->Eof) {
			ProcMess();
			if (CheckExit()) {
				throw EAbort(IDS_LOG_ERROR_TERMINATED_IN_WORK_PROGRESS);
			}

			Train = new TOracleTrain();

			Train->TrainNum = Trim(Query->FieldByName("RWNUM")->AsString);
			Train->DateTime = Query->FieldByName("DATETIME")->AsDateTime;
			Train->VanCount = Query->FieldByName("VAN_COUNT")->AsInteger;

			FTrainList->Add(Train);

			Query->Next();
		}
	}
	__finally {
		Query->Free();
	}

	Connection->Close();
}

// ---------------------------------------------------------------------------
