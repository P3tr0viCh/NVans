// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsSQL.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansTDBOracleLoadTrain.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOracleLoadTrain::TDBOracleLoadTrain
	(TConnectionInfo * ConnectionInfo, TKeyOracleTrain * KeyOracleTrain,
	bool WithJoin) : TDatabaseOperation(ConnectionInfo) {
	FVanList = new TOracleVanList();

	FKeyOracleTrain = new TKeyOracleTrain();

	FKeyOracleTrain->Assign(KeyOracleTrain);

	FWithJoin = WithJoin;
}

// ---------------------------------------------------------------------------
__fastcall TDBOracleLoadTrain::~TDBOracleLoadTrain() {
	FKeyOracleTrain->Free();
	FVanList->Free();
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrain::OperationStart() {
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrain::OperationEndOK() {
	WriteToLog(Format(IDS_LOG_ORACLE_LOAD_TRAIN_OK,
		ARRAYOFCONST((VanList->Count))));
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrain::OperationEndFail() {
	WriteToLog(Format(IDS_LOG_ORACLE_LOAD_TRAIN_FAIL, ErrorMessage));
}

// ---------------------------------------------------------------------------
void TDBOracleLoadTrain::Operation() {
	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		String QueryText;

		if (WithJoin) {
			QueryText = SQLMake(QueryText, IDS_SQL_SELECT);
			QueryText =
				SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAIN_JOIN_SELECT);
			QueryText = SQLMake(QueryText, IDS_SQL_FROM);
			QueryText = SQLMake(QueryText, "(");
			QueryText = SQLMake(QueryText, "(");
			QueryText = SQLMake(QueryText, IDS_SQL_SELECT);
			QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAIN_SELECT);
			QueryText = SQLMake(QueryText, IDS_SQL_FROM);
			QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TABLE);
			QueryText = SQLMake(QueryText, IDS_SQL_WHERE);
			QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAIN_WHERE);
			QueryText = SQLMake(QueryText, ") A");
			QueryText = SQLMake(QueryText, IDS_SQL_JOIN);
			QueryText = SQLMake(QueryText, "(");
			QueryText = SQLMake(QueryText, IDS_SQL_SELECT);
			QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAIN_SELECT);
			QueryText = SQLMake(QueryText, IDS_SQL_FROM);
			QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TABLE);
			QueryText = SQLMake(QueryText, ") B");
			QueryText =
				SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAIN_JOIN_JOIN);
			QueryText = SQLMake(QueryText, ")");
			QueryText = SQLMake(QueryText, IDS_SQL_ORDER);
			QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAIN_ORDER);
		}
		else {
			QueryText = SQLMake(QueryText, IDS_SQL_SELECT);
			QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAIN_SELECT);
			QueryText = SQLMake(QueryText, IDS_SQL_FROM);
			QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TABLE);
			QueryText = SQLMake(QueryText, IDS_SQL_WHERE);
			QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAIN_WHERE);
			QueryText = SQLMake(QueryText, IDS_SQL_ORDER);
			QueryText = SQLMake(QueryText, IDS_SQL_ORACLE_NVANS_TRAIN_ORDER);
		}

		Query->SQL->Text = QueryText;

#ifdef SQL_TO_LOG
		WriteToLog(Query->SQL->Text);
#endif

		SQLGetParam(Query, "RWNUM", ftFixedWideChar)->Value =
			KeyOracleTrain->TrainNum;
		SQLGetParam(Query, "NDATETIME", ftDateTime)->Value =
			KeyOracleTrain->DateTime;

#ifdef SQL_TO_LOG
		WriteToLog("PARAMS: RWNUM = " + KeyOracleTrain->TrainNum + ", " +
			"DATETIME = " + DateToStr(KeyOracleTrain->DateTime));
#endif

		Query->Open();

		TOracleVan * Van;

		while (!Query->Eof) {
			ProcMess();
			if (CheckExit()) {
				throw EAbort(IDS_LOG_ERROR_TERMINATED_IN_WORK_PROGRESS);
			}

			Van = new TOracleVan();

			Van->Num = Query->FieldByName("NUM")->AsInteger;

			Van->InvoiceDateTime = Query->FieldByName("NDATETIME")->AsDateTime;

			Van->VanNum = Trim(Query->FieldByName("INVNUM")->AsString);

			Van->CargoType = Trim(Query->FieldByName("CARGOTYPE")->AsString);

			Van->InvoiceNum = Trim(Query->FieldByName("INVOICE_NUM")->AsString);

			Van->InvoiceSupplier =
				Trim(Query->FieldByName("INVOICE_SUPPLIER")->AsString);
			Van->InvoiceRecipient =
				Trim(Query->FieldByName("INVOICE_CONSIGN")->AsString);

			Van->DepartStation =
				Trim(Query->FieldByName("DEPART_STATION")->AsString);
			Van->PurposeStation =
				Trim(Query->FieldByName("PURPOSE_STATION")->AsString);

			Van->Carrying = Query->FieldByName("CARRYING")->AsInteger * 1000;
			Van->TareT = Query->FieldByName("TARE_T")->AsInteger * 100;

			Van->InvoiceNetto = Query->FieldByName("INVOICE_NETTO")->AsInteger;
			Van->InvoiceTare = Query->FieldByName("INVOICE_TARE")
				->AsInteger * 100;

			FVanList->Add(Van);

			Query->Next();
		}
	}
	__finally {
		Query->Free();

		Connection->Close();
	}
}

// ---------------------------------------------------------------------------
