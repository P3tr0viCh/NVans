// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsSQL.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansTDBLocalSaveVanProps.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#define BRUTTO_BIT 0x08

// ---------------------------------------------------------------------------
__fastcall TDBLocalSaveVanProps::TDBLocalSaveVanProps
	(TConnectionInfo * ConnectionInfo, TOracleVanList * VanList)
	: TDatabaseOperation(ConnectionInfo) {
	FVanList = new TOracleVanList();

	InsertCount = 0;
	UpdateCount = 0;

	FVanList->Assign(VanList);
}

// ---------------------------------------------------------------------------
__fastcall TDBLocalSaveVanProps::~TDBLocalSaveVanProps() {
	FVanList->Free();
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVanProps::OperationStart() {
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVanProps::OperationEndOK() {
	WriteToLog(Format(IDS_LOG_LOCAL_SAVE_VANPROPS_OK,
		ARRAYOFCONST((InsertCount, UpdateCount))));
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVanProps::OperationEndFail() {
	WriteToLog(Format(IDS_LOG_LOCAL_SAVE_VANPROPS_FAIL, ErrorMessage));
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

		String QueryFindText = LoadStr(IDS_SQL_SELECT);
		QueryFindText = SQLMake(QueryFindText, IDS_SQL_LOCAL_PVANS_SELECT);
		QueryFindText = SQLMake(QueryFindText, IDS_SQL_FROM);
		QueryFindText = SQLMake(QueryFindText, IDS_SQL_LOCAL_PVANS_TABLE);
		QueryFindText = SQLMake(QueryFindText, IDS_SQL_WHERE);
		QueryFindText = SQLMake(QueryFindText, IDS_SQL_LOCAL_PVANS_WHERE);

		String QueryUpdateText = LoadStr(IDS_SQL_UPDATE);
		QueryUpdateText = SQLMake(QueryUpdateText, IDS_SQL_LOCAL_PVANS_TABLE);
		QueryUpdateText = SQLMake(QueryUpdateText, IDS_SQL_SET);
		QueryUpdateText = SQLMake(QueryUpdateText, IDS_SQL_LOCAL_PVANS_UPDATE);
		QueryUpdateText = SQLMake(QueryUpdateText, IDS_SQL_WHERE);
		QueryUpdateText = SQLMake(QueryUpdateText, IDS_SQL_LOCAL_PVANS_WHERE);

		String QueryInsertText = LoadStr(IDS_SQL_INSERT);
		QueryInsertText = SQLMake(QueryInsertText, IDS_SQL_LOCAL_PVANS_TABLE);
		QueryInsertText = SQLMake(QueryInsertText,
			IDS_SQL_LOCAL_PVANS_INSERT_FIELDS);
		QueryInsertText = SQLMake(QueryInsertText, IDS_SQL_VALUES);
		QueryInsertText = SQLMake(QueryInsertText,
			IDS_SQL_LOCAL_PVANS_INSERT_VALUES);

		QueryFind->SQL->Text = QueryFindText;
		QueryUpdate->SQL->Text = QueryUpdateText;
		QueryInsert->SQL->Text = QueryInsertText;

#ifdef SQL_TO_LOG
		WriteToLog(QueryFind->SQL->Text);
		WriteToLog(QueryUpdate->SQL->Text);
		WriteToLog(QueryInsert->SQL->Text);
#endif

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

#ifdef SQL_TO_LOG
			WriteToLog("SELECT PARAMS: INVNUM = " +
				VarToStr(pFindVanNum->Value));
#endif
			QueryFind->Open();
			DoUpdate = QueryFind->RecordCount > 0;
			QueryFind->Close();

			if (DoUpdate) {
				pUpdateVanNum->Value = VanList->Items[i]->VanNum;

				pUpdateCarrying->Value = VanList->Items[i]->Carrying;
				pUpdateLoadNorm->Value = VanList->Items[i]->Carrying;
				pUpdateTareT->Value = VanList->Items[i]->TareT;

#ifdef SQL_TO_LOG
				WriteToLog("UPDATE PARAMS: CARRYING = " +
					VarToStr(pUpdateCarrying->Value) + ", " + "TARE_T = " +
					VarToStr(pUpdateTareT->Value) + ", " + "IDATETIME* = " +
					VarToStr(DateTime));
#endif
			}
			else {
				pInserTLocalVanNum->Value = VanList->Items[i]->VanNum;

				pInsertCarrying->Value = VanList->Items[i]->Carrying;
				pInsertLoadNorm->Value = VanList->Items[i]->Carrying;
				pInsertTareT->Value = VanList->Items[i]->TareT;

#ifdef SQL_TO_LOG
				WriteToLog("INSERT PARAMS: CARRYING = " +
					VarToStr(pInsertCarrying->Value) + ", " + "TARE_T = " +
					VarToStr(pInsertTareT->Value) + ", " + "IDATETIME* = " +
					VarToStr(DateTime));
#endif
			}

			if (DoUpdate) {
				QueryUpdate->ExecSQL();

				UpdateCount++;
			}
			else {
				QueryInsert->ExecSQL();

				InsertCount++;
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
