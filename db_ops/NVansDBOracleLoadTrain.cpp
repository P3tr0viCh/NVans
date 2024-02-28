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
	IDBOperationEvent * DBOperationEvent, TKeyOracleTrain * KeyOracleTrain)
	: TDBOperation(DBConnection, DBOperationEvent) {
	FVanList = new TOracleVanList();

	FKeyOracleTrain = new TKeyOracleTrain();

	FKeyOracleTrain->Assign(KeyOracleTrain);
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

		SQLSetText(Query, IDS_SQL_ORACLE_NVANS_TRAIN);

		if (SQLToLog) {
			WriteToLog(Query->SQL->Text);
		}

		SQLGetParam(Query, "RWNUM", ftFixedWideChar)->Value =
			KeyOracleTrain->TrainNum;
		SQLGetParam(Query, "NDATETIME", ftFixedWideChar)->Value =
			DateTimeToSQLStr(KeyOracleTrain->DateTime);

		if (SQLToLog) {
			WriteToLog(SQLParamsToStr(Query));
		}

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
			Van->CargoTypeCode =
				StrToIntDef(Trim(Query->FieldByName("CARGOTYPE_CODE")
				->AsString), DEFAULT_CODE);

			Van->InvoiceNum = Trim(Query->FieldByName("INVOICE_NUM")->AsString);

			Van->InvoiceSupplier =
				Trim(Query->FieldByName("INVOICE_SUPPLIER")->AsString);
			Van->InvoiceRecipient =
				Trim(Query->FieldByName("INVOICE_CONSIGN")->AsString);

			Van->DepartStation =
				Trim(Query->FieldByName("DEPART_STATION")->AsString);
			Van->PurposeStation =
				Trim(Query->FieldByName("PURPOSE_STATION")->AsString);

			Van->Carrying = Query->FieldByName("CARRYING")->AsInteger;
			Van->TareT = Query->FieldByName("TARE_T")->AsInteger;

			Van->InvoiceNetto = Query->FieldByName("INVOICE_NETTO")->AsInteger;
			Van->InvoiceTare = Query->FieldByName("INVOICE_TARE")->AsInteger;

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
