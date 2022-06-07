// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansTDBLocalLoadTrains.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#define BRUTTO_BIT 0x08
#define MIXED_BIT  0x01000000

// ---------------------------------------------------------------------------
__fastcall TDBLocalLoadTrains::TDBLocalLoadTrains
	(TConnectionInfo * ConnectionInfo, TFilterLocalTrains * Filter)
	: TDatabaseOperation(ConnectionInfo) {
	FFilter = new TFilterLocalTrains();

	FTrainList = new TLocalTrainList();

	FFilter->Assign(Filter);
}

// ---------------------------------------------------------------------------
__fastcall TDBLocalLoadTrains::~TDBLocalLoadTrains() {
	FTrainList->Free();
	FFilter->Free();
}

// ---------------------------------------------------------------------------
void TDBLocalLoadTrains::OperationStart() {
}

// ---------------------------------------------------------------------------
void TDBLocalLoadTrains::OperationEndOK() {
	WriteToLog(Format(IDS_LOG_LOCAL_LOAD_TRAINS_OK,
		ARRAYOFCONST((TrainList->Count))));
}

// ---------------------------------------------------------------------------
void TDBLocalLoadTrains::OperationEndFail() {
	WriteToLog(Format(IDS_LOG_LOCAL_LOAD_TRAINS_FAIL, ErrorMessage));
}

// ---------------------------------------------------------------------------
void TDBLocalLoadTrains::Operation() {
	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		String QueryText;

		QueryText = SQLMake(QueryText, IDS_SQL_SELECT);
		QueryText = SQLMake(QueryText, IDS_SQL_LOCAL_MTRAINS_SELECT);
		QueryText = SQLMake(QueryText, IDS_SQL_FROM);
		QueryText = SQLMake(QueryText, IDS_SQL_LOCAL_MTRAINS_TABLE);
		QueryText = SQLMake(QueryText, IDS_SQL_WHERE);
		QueryText = SQLMake(QueryText, IDS_SQL_LOCAL_MTRAINS_WHERE);
		QueryText = SQLMake(QueryText, IDS_SQL_ORDER);
		QueryText = SQLMake(QueryText, IDS_SQL_LOCAL_MTRAINS_ORDER);

		Query->SQL->Text = QueryText;

#ifdef SQL_TO_LOG
		WriteToLog(Query->SQL->Text);
#endif

		GetParam(Query, "DATE_FROM", ftDate)->Value = Filter->Date;
		GetParam(Query, "DATE_TO", ftDate)->Value = Filter->Date + 1;

#ifdef SQL_TO_LOG
		WriteToLog("PARAMS: DATE_FROM = " + DateToStr(Filter->Date) + ", " +
			"DATE_TO = " + DateToStr(Filter->Date + 1));
#endif

		Query->Open();

		TLocalTrain * Train;

		int TrainStatus;

		while (!Query->Eof) {
			ProcMess();
			if (CheckExit()) {
				throw EAbort(IDS_LOG_ERROR_TERMINATED_IN_WORK_PROGRESS);
			}

			Train = new TLocalTrain();

			Train->TrainNum = Trim(Query->FieldByName("trnum")->AsString);
			Train->DateTime = Query->FieldByName("bdatetime")->AsDateTime;
			Train->VanCount = Query->FieldByName("nvans")->AsInteger;

			TrainStatus = Query->FieldByName("status")->AsInteger;

			if (TrainStatus & BRUTTO_BIT) {
				Train->WeightType = wtBrutto;
			}
			else {
                // У Авитека ошибка: после обновления таблицы поездов, все смешанные составы становятся порожними
				if (TrainStatus & MIXED_BIT) {
					Train->WeightType = wtMixed;
				}
				else {
					Train->WeightType = wtTare;
				}
			}

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
