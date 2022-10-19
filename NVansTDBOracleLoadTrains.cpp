// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsSQL.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansTDBOracleLoadTrains.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOracleLoadTrains::TDBOracleLoadTrains
	(TConnectionInfo * ConnectionInfo, TFilterOracleTrains * Filter)
	: TDatabaseOperation(ConnectionInfo) {
	FFilter = new TFilterOracleTrains();

	FTrainList = new TOracleTrainList();

	FFilter->Assign(Filter);
}

// ---------------------------------------------------------------------------
__fastcall TDBOracleLoadTrains::~TDBOracleLoadTrains() {
	FTrainList->Free();
	FFilter->Free();
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

		bool SearchByDate = Filter->VanNum.IsEmpty();

		String QueryText;

		QueryText = SQLMake(QueryText, IDS_SQL_SELECT);
		QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAINS_SELECT);
		QueryText = SQLMake(QueryText, IDS_SQL_FROM);
		QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TABLE);
		QueryText = SQLMake(QueryText, IDS_SQL_WHERE);
		if (SearchByDate) {
			QueryText =
				SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAINS_WHERE_DT);
		}
		else {
			QueryText =
				SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAINS_WHERE_IN_VN);
			QueryText = SQLMake(QueryText, "(");
			QueryText = SQLMake(QueryText, IDS_SQL_SELECT);
			QueryText =
				SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAINS_SELECT_VN);
			QueryText = SQLMake(QueryText, IDS_SQL_FROM);
			QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TABLE);
			QueryText = SQLMake(QueryText, IDS_SQL_WHERE);
			QueryText =
				SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAINS_WHERE_VN);
			QueryText = SQLMake(QueryText, ")");
		}
		QueryText = SQLMake(QueryText, IDS_SQL_GROUP);
		QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAINS_GROUP);
		QueryText = SQLMake(QueryText, IDS_SQL_ORDER);
		QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAINS_ORDER);

		Query->SQL->Text = QueryText;

#ifdef SQL_TO_LOG
		WriteToLog(Query->SQL->Text);
#endif

		if (SearchByDate) {
			SQLGetParam(Query, "DATE_FROM", ftDate)->Value = Filter->Date;
			SQLGetParam(Query, "DATE_TO", ftDate)->Value = Filter->Date + 1;

#ifdef SQL_TO_LOG
			WriteToLog("PARAMS: DATE_FROM = " + DateToStr(Filter->Date) + ", " +
				"DATE_TO = " + DateToStr(Filter->Date + 1));
#endif
		}
		else {
			SQLGetParam(Query, "INVNUM", ftFixedWideChar)->Value = Filter->VanNum;
			SQLGetParam(Query, "DATE_FROM", ftDate)->Value = Filter->Date - 30;

#ifdef SQL_TO_LOG
			WriteToLog("PARAMS: INVNUM = " + Filter->VanNum + ", " +
				"DATE_FROM = " + DateToStr(Filter->Date - 30));
#endif
		}

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

		Connection->Close();
	}
}

// ---------------------------------------------------------------------------
