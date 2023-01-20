// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsSQL.h>
#include <UtilsMisc.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansDBLocalSaveVanProps.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#define BRUTTO_BIT 0x08

// ---------------------------------------------------------------------------
__fastcall TDBLocalSaveVanProps::TDBLocalSaveVanProps
	(TDBConnection * DBConnection, IDBOperationEvent * DBOperationEvent,
	TOracleVanList * VanList) : TDBOperation(DBConnection, DBOperationEvent) {
	FVanList = new TOracleVanList();

	FInsertCount = 0;
	FUpdateCount = 0;

	FVanList->Assign(VanList);
}

// ---------------------------------------------------------------------------
__fastcall TDBLocalSaveVanProps::~TDBLocalSaveVanProps() {
	FVanList->Free();
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVanProps::Operation() {
	Connection->Open();

	TADOQuery * QueryFind = new TADOQuery(NULL);
	TADOQuery * QueryUpdate = new TADOQuery(NULL);
	TADOQuery * QueryInsert = new TADOQuery(NULL);
	try {
		QueryFind->Connection = Connection;
		QueryUpdate->Connection = Connection;
		QueryInsert->Connection = Connection;

		String DateTime = DateTimeToSQLStr(Now());

		bool DoUpdate;

		SQLSetText(QueryFind, IDS_SQL_LOCAL_PVANS_SELECT);
		SQLSetText(QueryUpdate, IDS_SQL_LOCAL_PVANS_UPDATE);
		SQLSetText(QueryInsert, IDS_SQL_LOCAL_PVANS_INSERT);

		if (SQLToLog) {
			WriteToLog(QueryFind->SQL->Text);
			WriteToLog(QueryUpdate->SQL->Text);
			WriteToLog(QueryInsert->SQL->Text);
		}

		TParameter * pFindVanNum = SQLGetParam(QueryFind, "INVNUM", ftString);
		TParameter * pUpdateVanNum =
			SQLGetParam(QueryUpdate, "INVNUM", ftString);
		TParameter * pInserTLocalVanNum =
			SQLGetParam(QueryInsert, "INVNUM", ftString);

		TParameter * pUpdateCarrying =
			SQLGetParam(QueryUpdate, "CARRYING", ftInteger);
		SQLGetParam(QueryUpdate, "ISCALES_CARRYING", ftInteger)->Value = 0;
		SQLGetParam(QueryUpdate, "IDATETIME_CARRYING", ftString)->Value =
			DateTime;
		TParameter * pUpdateLoadNorm =
			SQLGetParam(QueryUpdate, "LOADNORM", ftInteger);
		SQLGetParam(QueryUpdate, "ISCALES_LOADNORM", ftInteger)->Value = 0;
		SQLGetParam(QueryUpdate, "IDATETIME_LOADNORM", ftString)->Value =
			DateTime;
		TParameter * pUpdateTareT = SQLGetParam(QueryUpdate, "TARE_T",
			ftInteger);
		SQLGetParam(QueryUpdate, "ISCALES_TARE_T", ftInteger)->Value = 0;
		SQLGetParam(QueryUpdate, "IDATETIME_TARE_T", ftString)->Value =
			DateTime;

		TParameter * pInsertCarrying =
			SQLGetParam(QueryInsert, "CARRYING", ftInteger);
		SQLGetParam(QueryInsert, "ISCALES_CARRYING", ftInteger)->Value = 0;
		SQLGetParam(QueryInsert, "IDATETIME_CARRYING", ftString)->Value =
			DateTime;
		TParameter * pInsertLoadNorm =
			SQLGetParam(QueryInsert, "LOADNORM", ftInteger);
		SQLGetParam(QueryInsert, "ISCALES_LOADNORM", ftInteger)->Value = 0;
		SQLGetParam(QueryInsert, "IDATETIME_LOADNORM", ftString)->Value =
			DateTime;
		TParameter * pInsertTareT = SQLGetParam(QueryInsert, "TARE_T",
			ftInteger);
		SQLGetParam(QueryInsert, "ISCALES_TARE_T", ftInteger)->Value = 0;
		SQLGetParam(QueryInsert, "IDATETIME_TARE_T", ftString)->Value =
			DateTime;

		for (int i = 0; i < VanList->Count; i++) {
			ProcMess();
			if (CheckExit()) {
				throw EAbort(IDS_LOG_ERROR_TERMINATED_IN_WORK_PROGRESS);
			}

			pFindVanNum->Value = VanList->Items[i]->VanNum;

			if (SQLToLog) {
				WriteToLog(SQLParamsToStr(QueryFind));
			}

			QueryFind->Open();
			DoUpdate = QueryFind->RecordCount > 0;
			QueryFind->Close();

			if (DoUpdate) {
				pUpdateVanNum->Value = VanList->Items[i]->VanNum;

				pUpdateCarrying->Value = VanList->Items[i]->Carrying;
				pUpdateLoadNorm->Value = VanList->Items[i]->Carrying;
				pUpdateTareT->Value = VanList->Items[i]->TareT;

				if (SQLToLog) {
					WriteToLog(SQLParamsToStr(QueryUpdate));
				}
			}
			else {
				pInserTLocalVanNum->Value = VanList->Items[i]->VanNum;

				pInsertCarrying->Value = VanList->Items[i]->Carrying;
				pInsertLoadNorm->Value = VanList->Items[i]->Carrying;
				pInsertTareT->Value = VanList->Items[i]->TareT;

				if (SQLToLog) {
					WriteToLog(SQLParamsToStr(QueryInsert));
				}
			}

			if (DoUpdate) {
				QueryUpdate->ExecSQL();

				FUpdateCount++;
			}
			else {
				QueryInsert->ExecSQL();

				FInsertCount++;
			}
		}
	}
	__finally {
		QueryInsert->Free();
		QueryUpdate->Free();
		QueryFind->Free();

		Connection->Close();
	}
}

// ---------------------------------------------------------------------------
