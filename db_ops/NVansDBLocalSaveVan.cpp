// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsSQL.h>
#include <UtilsMisc.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansDBLocalSaveVan.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#define BRUTTO_BIT 0x08

// ---------------------------------------------------------------------------
__fastcall TDBLocalSaveVan::TDBLocalSaveVan(TDBConnection * DBConnection,
	IDBOperationEvent * DBOperationEvent)
	: TDBOperation(DBConnection, DBOperationEvent) {
	FVan = new TLocalVan();
}

// ---------------------------------------------------------------------------
__fastcall TDBLocalSaveVan::~TDBLocalSaveVan() {
	FVan->Free();
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVan::SetVan(TLocalVan * Van) {
	FVan->Assign(Van);
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVan::Operation() {
	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		SQLSetText(Query, IDS_SQL_LOCAL_MVANS_UPDATE);

		if (SQLToLog) {
			WriteToLog(Query->SQL->Text);
		}

		SQLGetParam(Query, "ID", ftInteger)->Value = Van->ID;

		SQLGetParam(Query, "INVNUM", ftString)->Value = Van->VanNum;

		SQLGetParam(Query, "CARRYING", ftInteger)->Value = Van->Carrying;
		SQLGetParam(Query, "LOADNORM", ftInteger)->Value = Van->Carrying;

		SQLGetParam(Query, "TARE", ftInteger)->Value = Van->Tare;
		SQLGetParam(Query, "TARE_T", ftInteger)->Value = Van->TareT;
		SQLGetParam(Query, "ISCALES_TARE", ftInteger)->Value =
			Van->TareScaleNum;
		SQLGetParam(Query, "IDATETIME_TARE", ftString)->Value =
			DateTimeToSQLStr(Van->TareDateTime);

		SQLGetParam(Query, "NETTO", ftInteger)->Value = Van->Netto;

		SQLGetParam(Query, "OVERLOAD", ftInteger)->Value = Van->Overload;

		SQLGetParam(Query, "CARGOTYPE", ftString)->Value = Van->CargoType;
		if (Van->CargoTypeCode == DEFAULT_CODE) {
			SQLGetParam(Query, "CARGOTYPE_CODE", ftString)->Value = Null();
		}
		else {
			SQLGetParam(Query, "CARGOTYPE_CODE", ftInteger)->Value =
				Van->CargoTypeCode;
		}
		
		SQLGetParam(Query, "DEPART_STATION", ftString)->Value =
			Van->DepartStation;
		SQLGetParam(Query, "DEPART_STATION_CODE", ftString)->Value = Null();
		SQLGetParam(Query, "PURPOSE_STATION", ftString)->Value =
			Van->PurposeStation;
		SQLGetParam(Query, "PURPOSE_STATION_CODE", ftString)->Value = Null();

		SQLGetParam(Query, "INVOICE_NUM", ftString)->Value = Van->InvoiceNum;

		SQLGetParam(Query, "INVOICE_SUPPLIER", ftString)->Value =
			Van->InvoiceSupplier;
		SQLGetParam(Query, "INVOICE_SUPPLIER_CODE", ftString)->Value = Null();
		SQLGetParam(Query, "INVOICE_CONSIGN", ftString)->Value =
			Van->InvoiceRecipient;
		SQLGetParam(Query, "INVOICE_CONSIGN_CODE", ftString)->Value = Null();

		SQLGetParam(Query, "INVOICE_NETTO", ftInteger)->Value =
			Van->InvoiceNetto;
		SQLGetParam(Query, "INVOICE_TARE", ftInteger)->Value = Van->InvoiceTare;

		// TODO
		SQLGetParam(Query, "DISBALONG", ftInteger)->Value = 0;
		SQLGetParam(Query, "DISBCROSS", ftInteger)->Value = 0;
		SQLGetParam(Query, "TOL_DISBALONG", ftInteger)->Value = 0;
		SQLGetParam(Query, "TOL_DISBCROSS", ftInteger)->Value = 0;

		if (SQLToLog) {
			WriteToLog(SQLParamsToStr(Query));
		}

		Query->ExecSQL();

		if (SQLToLog) {
			WriteToLog("RowsAffected: " + IntToStr(Query->RowsAffected));
		}

		if (Query->RowsAffected != 1) {
			throw Exception("record with id=" + IntToStr(Van->ID) +
				" not exists");
		}
	}
	__finally {
		Query->Free();

		Connection->Close();
	}
}

// ---------------------------------------------------------------------------
