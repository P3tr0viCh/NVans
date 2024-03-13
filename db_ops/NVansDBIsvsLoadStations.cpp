// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsSQL.h>
#include <UtilsMisc.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansDBIsvsLoadStations.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBIsvsLoadStations::TDBIsvsLoadStations
	(TDBConnection * DBConnection, IDBOperationEvent * DBOperationEvent,
	TCodeNamePairList * StationList)
	: TDBOperation(DBConnection, DBOperationEvent) {
	FStationList = StationList;
}

// ---------------------------------------------------------------------------
__fastcall TDBIsvsLoadStations::~TDBIsvsLoadStations() {
}

// ---------------------------------------------------------------------------
void TDBIsvsLoadStations::Operation() {
	if (StationList->Count == 0) {
		return;
	}

	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		SQLSetText(Query, IDS_SQL_ISVS_LST_STATION_SELECT);

		if (SQLToLog) {
			WriteToLog(Query->SQL->Text);
		}

		for (int i = 0; i < StationList->Count; i++) {
			ProcMess();
			if (CheckExit()) {
				throw EAbort(IDS_LOG_ERROR_TERMINATED_IN_WORK_PROGRESS);
			}

			StationList->Items[i]->Name = "";

			SQLGetParam(Query, "STATION_CODE", ftInteger)->Value =
				StationList->Items[i]->Code;

			if (SQLToLog) {
				WriteToLog(SQLParamsToStr(Query));
			}

			Query->Open();

			if (Query->RecordCount > 0) {
				StationList->Items[i]->Name =
					Query->FieldByName("station")->AsString;
			}

			Query->Close();
		}
	}
	__finally {
		Query->Free();

		Connection->Close();
	}
}

// ---------------------------------------------------------------------------
