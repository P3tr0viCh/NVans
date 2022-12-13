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
	TFilterOracleTrains * Filter)
	: TDBOperation(DBConnection, DBOperationEvent) {
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
			Query->SQL->Text = SQLLoad(IDS_SQL_ORACLE_NVANS_TRAINS_BY_DT);
		}
		else {
			Query->SQL->Text = SQLLoad(IDS_SQL_ORACLE_NVANS_TRAINS_BY_VN);
		}

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
			SQLGetParam(Query, "INVNUM", ftFixedWideChar)->Value =
				Filter->VanNum;

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
