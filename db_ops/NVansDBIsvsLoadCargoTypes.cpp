// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsSQL.h>
#include <UtilsMisc.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansDBIsvsLoadCargoTypes.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBIsvsLoadCargoTypes::TDBIsvsLoadCargoTypes
	(TDBConnection * DBConnection, IDBOperationEvent * DBOperationEvent,
	TCodeNamePairList * CargoTypeList)
	: TDBOperation(DBConnection, DBOperationEvent) {
	FCargoTypeList = CargoTypeList;
}

// ---------------------------------------------------------------------------
__fastcall TDBIsvsLoadCargoTypes::~TDBIsvsLoadCargoTypes() {
}

// ---------------------------------------------------------------------------
void TDBIsvsLoadCargoTypes::Operation() {
	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		SQLSetText(Query, IDS_SQL_ISVS_CARGOTYPE_SELECT);

		if (SQLToLog) {
			WriteToLog(Query->SQL->Text);
		}

		for (int i = 0; i < CargoTypeList->Count; i++) {
			ProcMess();
			if (CheckExit()) {
				throw EAbort(IDS_LOG_ERROR_TERMINATED_IN_WORK_PROGRESS);
			}

			CargoTypeList->Items[i]->Name = "";

			SQLGetParam(Query, "CARGOTYPE_CODE", ftInteger)->Value =
				CargoTypeList->Items[i]->Code;

			if (SQLToLog) {
				WriteToLog(SQLParamsToStr(Query));
			}

			Query->Open();

			if (Query->RecordCount > 0) {
				CargoTypeList->Items[i]->Name =
					Query->FieldByName("cargotype")->AsString;
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
