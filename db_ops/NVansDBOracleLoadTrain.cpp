// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsSQL.h>
#include <UtilsMisc.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansDBOracleLoadTrain.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOracleLoadTrain::TDBOracleLoadTrain(TDBConnection * DBConnection,
	IDBOperationEvent * DBOperationEvent, TKeyOracleTrain * KeyOracleTrain,
	bool WithJoin) : TDBOperation(DBConnection, DBOperationEvent) {
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
void TDBOracleLoadTrain::Operation() {
	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		if (WithJoin) {
			Query->SQL->Text = SQLLoad(IDS_SQL_ORACLE_NVANS_TRAIN_JOIN);
		}
		else {
			Query->SQL->Text = SQLLoad(IDS_SQL_ORACLE_NVANS_TRAIN);
		}

#ifdef SQL_TO_LOG
		WriteToLog(Query->SQL->Text);
#endif

		SQLGetParam(Query, "RWNUM", ftFixedWideChar)->Value =
			KeyOracleTrain->TrainNum;
		SQLGetParam(Query, "NDATETIME", ftFixedWideChar)->Value =
			DateTimeToSQLStr(KeyOracleTrain->DateTime);

#ifdef SQL_TO_LOG
		WriteToLog("PARAMS: RWNUM = " + KeyOracleTrain->TrainNum + ", " +
			"DATETIME = " + DateTimeToStr(KeyOracleTrain->DateTime));
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

			Van->VanNum =
				NormalizeVanNumView(Trim(Query->FieldByName("INVNUM")
				->AsString));

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
