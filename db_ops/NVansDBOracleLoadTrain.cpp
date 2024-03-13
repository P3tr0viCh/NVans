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
int TDBOracleLoadTrain::QueryCode(TADOQuery * Query, String FieldName) {
	String CargoTypeCode = Trim(Query->FieldByName(FieldName)->AsString);

	return StrToIntDef(CargoTypeCode, DEFAULT_CODE);
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

			// ---------------------------------------------------------------
			Van->CargoType = Trim(Query->FieldByName("CARGOTYPE")->AsString);

			Van->CargoTypeCode = QueryCode(Query, "CARGOTYPE_CODE");
			if (Van->CargoTypeCode > 1000 && Van->CargoTypeCode < 100000) {
				Van->CargoTypeCode = Van->CargoTypeCode * 10 +
					GetRusControlNumber(Van->CargoTypeCode, 5);
			}

			// ---------------------------------------------------------------
			Van->InvoiceNum = Trim(Query->FieldByName("INVOICE_NUM")->AsString);

			Van->InvoiceSupplier =
				Trim(Query->FieldByName("INVOICE_SUPPLIER")->AsString);
			Van->InvoiceRecipient =
				Trim(Query->FieldByName("INVOICE_CONSIGN")->AsString);

			// ---------------------------------------------------------------
			Van->DepartStation =
				Trim(Query->FieldByName("DEPART_STATION")->AsString);
			Van->DepartStationCode = QueryCode(Query, "DEPART_STATION_CODE");

			// UIT number
			if (Van->DepartStationCode == 999) {
				Van->DepartStationCode = 814208;
			}

			// ---------------------------------------------------------------
			Van->PurposeStation =
				Trim(Query->FieldByName("PURPOSE_STATION")->AsString);
			Van->PurposeStationCode = QueryCode(Query, "PURPOSE_STATION_CODE");

			// ---------------------------------------------------------------
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
