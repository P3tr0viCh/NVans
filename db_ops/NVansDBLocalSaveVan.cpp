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
void SQLSetInt(TADOQuery * Query, String Name, int Value) {
	SQLGetParam(Query, Name, ftInteger)->Value = Value;
}

// ---------------------------------------------------------------------------
void SQLSetCode(TADOQuery * Query, String Name, int Code) {
	if (Code == DEFAULT_CODE) {
		SQLGetParam(Query, Name, ftString)->Value = Null();
	}
	else {
		SQLSetInt(Query, Name, Code);
	}
}

// ---------------------------------------------------------------------------
void SQLSetString(TADOQuery * Query, String Name, String Value) {
	SQLGetParam(Query, Name, ftString)->Value = Value;
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

		SQLSetInt(Query, "ID", Van->ID);

		SQLSetString(Query, "INVNUM", Van->VanNum);

		SQLSetInt(Query, "CARRYING", Van->Carrying);
		SQLSetInt(Query, "LOADNORM", Van->Carrying);

		SQLSetInt(Query, "TARE", Van->Tare);
		SQLSetInt(Query, "TARE_T", Van->TareT);
		SQLSetInt(Query, "ISCALES_TARE", Van->TareScaleNum);
		SQLSetString(Query, "IDATETIME_TARE",
			DateTimeToSQLStr(Van->TareDateTime));

		SQLSetInt(Query, "NETTO", Van->Netto);

		SQLSetInt(Query, "OVERLOAD", Van->Overload);

		SQLSetString(Query, "CARGOTYPE", Van->CargoType);
		SQLSetCode(Query, "CARGOTYPE_CODE", Van->CargoTypeCode);

		SQLSetString(Query, "DEPART_STATION", Van->DepartStation);
		SQLSetCode(Query, "DEPART_STATION_CODE", Van->DepartStationCode);
		SQLSetString(Query, "PURPOSE_STATION", Van->PurposeStation);
		SQLSetCode(Query, "PURPOSE_STATION_CODE", Van->PurposeStationCode);

		SQLSetString(Query, "INVOICE_NUM", Van->InvoiceNum);

		SQLSetString(Query, "INVOICE_SUPPLIER", Van->InvoiceSupplier);
		SQLSetCode(Query, "INVOICE_SUPPLIER_CODE", DEFAULT_CODE);
		SQLSetString(Query, "INVOICE_CONSIGN", Van->InvoiceRecipient);
		SQLSetCode(Query, "INVOICE_CONSIGN_CODE", DEFAULT_CODE);

		SQLSetInt(Query, "INVOICE_NETTO", Van->InvoiceNetto);
		SQLSetInt(Query, "INVOICE_TARE", Van->InvoiceTare);

		// TODO
		SQLSetInt(Query, "DISBALONG", 0);
		SQLSetInt(Query, "DISBCROSS", 0);
		SQLSetInt(Query, "TOL_DISBALONG", 0);
		SQLSetInt(Query, "TOL_DISBCROSS", 0);

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
