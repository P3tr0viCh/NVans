// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsSQL.h>
#include <UtilsMisc.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansDBOracleLoadTrains.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOracleLoadTrains::TDBOracleLoadTrains
	(TDBConnection * DBConnection, IDBOperationEvent * DBOperationEvent,
	TFilterOracleTrains * Filter) : TDBOperation(DBConnection, DBOperationEvent)
{
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
void TDBOracleLoadTrains::Operation() {
	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		bool SearchByDate =
			Filter->VanNum.IsEmpty() && Filter->InvoiceNum_1.IsEmpty();

		if (SearchByDate) {
			SQLSetText(Query, IDS_SQL_ORACLE_NVANS_TRAINS_BY_DT);
		}
		else {
			SQLSetText(Query, IDS_SQL_ORACLE_NVANS_TRAINS_BY_VN);
		}

		if (SQLToLog) {
			WriteToLog(Query->SQL->Text);
		}

		if (SearchByDate) {
			SQLGetParam(Query, "DATE_FROM", ftDate)->Value = Filter->Date;
			SQLGetParam(Query, "DATE_TO", ftDate)->Value = Filter->Date + 1;

			if (SQLToLog) {
				WriteToLog(SQLParamsToStr(Query));
			}
		}
		else {
			SQLGetParam(Query, "INVNUM", ftFixedWideChar)->Value =
				Filter->VanNum;

			if (SQLToLog) {
				WriteToLog(SQLParamsToStr(Query));
			}
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
			Train->DateTime = Query->FieldByName("NDATETIME")->AsDateTime;
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
