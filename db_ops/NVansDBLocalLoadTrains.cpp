// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsSQL.h>
#include <UtilsMisc.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansDBLocalLoadTrains.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#define BRUTTO_BIT 0x08
#define MIXED_BIT  0x01000000

// ---------------------------------------------------------------------------
__fastcall TDBLocalLoadTrains::TDBLocalLoadTrains(TDBConnection * DBConnection,
	IDBOperationEvent * DBOperationEvent, TFilterLocalTrains * Filter)
	: TDBOperation(DBConnection, DBOperationEvent) {
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
void TDBLocalLoadTrains::Operation() {
	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		SQLSetText(Query, IDS_SQL_LOCAL_MTRAINS_SELECT);

		if (SQLToLog) {
			WriteToLog(Query->SQL->Text);
		}

		SQLGetParam(Query, "DATE_FROM", ftDate)->Value = Filter->Date;
		SQLGetParam(Query, "DATE_TO", ftDate)->Value = Filter->Date + 1;

		if (SQLToLog) {
			WriteToLog(SQLParamsToStr(Query));
		}

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
