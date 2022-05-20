// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansTDBLocalSaveVan.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#define BRUTTO_BIT 0x08

// ---------------------------------------------------------------------------
__fastcall TDBLocalSaveVan::TDBLocalSaveVan(TConnectionInfo * ConnectionInfo)
	: TDatabaseOperation(ConnectionInfo) {
	FVan = new TVan();
}

// ---------------------------------------------------------------------------
__fastcall TDBLocalSaveVan::~TDBLocalSaveVan() {
	FVan->Free();
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVan::OperationStart() {
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVan::OperationEndOK() {
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVan::OperationEndFail() {
	WriteToLog(Format(IDS_LOG_LOCAL_SAVE_VANS_FAIL, ErrorMessage));
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVan::SetVan(TVan * Van) {
	FVan->Assign(Van);
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVan::Operation() {
	Connection->Open();

	TADOQuery * QueryUpdate = new TADOQuery(NULL);
	try {
		QueryUpdate->Connection = Connection;

		String DateTime = DateTimeToSQLStr(Now());

		String QueryUpdateText = LoadStr(IDS_SQL_UPDATE);
		QueryUpdateText = SQLMake(QueryUpdateText, IDS_SQL_LOCAL_MVANS_TABLE);
		QueryUpdateText = SQLMake(QueryUpdateText, IDS_SQL_SET);
		QueryUpdateText = SQLMake(QueryUpdateText, IDS_SQL_LOCAL_MVANS_UPDATE);
		QueryUpdateText = SQLMake(QueryUpdateText, IDS_SQL_WHERE);
		QueryUpdateText = SQLMake(QueryUpdateText,
			IDS_SQL_LOCAL_MVANS_UPDATE_WHERE);

		QueryUpdate->SQL->Text = QueryUpdateText;

#ifdef SQL_TO_LOG
		WriteToLog(QueryUpdate->SQL->Text);
#endif

		GetParam(QueryUpdate, "ID", ftInteger)->Value = Van->ID;

		GetParam(QueryUpdate, "INVNUM", ftString)->Value = Van->VanNum;

		GetParam(QueryUpdate, "CARRYING", ftInteger)->Value = Van->Carrying;
		GetParam(QueryUpdate, "LOADNORM", ftInteger)->Value = Van->Carrying;

		GetParam(QueryUpdate, "TARE_T", ftInteger)->Value = Van->TareT;
		GetParam(QueryUpdate, "ISCALES_TARE_T", ftInteger)->Value = 0;
		GetParam(QueryUpdate, "IDATETIME_TARE_T", ftString)->Value = DateTime;

		GetParam(QueryUpdate, "CARGOTYPE", ftString)->Value = Van->CargoType;
		GetParam(QueryUpdate, "CARGOTYPE_CODE", ftString)->Value = Null();

		GetParam(QueryUpdate, "DEPART_STATION", ftString)->Value =
			Van->DepartStation;
		GetParam(QueryUpdate, "DEPART_STATION_CODE", ftString)->Value = Null();
		GetParam(QueryUpdate, "PURPOSE_STATION", ftString)->Value =
			Van->PurposeStation;
		GetParam(QueryUpdate, "PURPOSE_STATION_CODE", ftString)->Value = Null();

		GetParam(QueryUpdate, "INVOICE_NUM", ftString)->Value = Van->InvoiceNum;

		GetParam(QueryUpdate, "INVOICE_SUPPLIER", ftString)->Value =
			Van->InvoiceSupplier;
		GetParam(QueryUpdate, "INVOICE_SUPPLIER_CODE", ftString)->Value =
			Null();
		GetParam(QueryUpdate, "INVOICE_CONSIGN", ftString)->Value =
			Van->InvoiceRecipient;
		GetParam(QueryUpdate, "INVOICE_CONSIGN_CODE", ftString)->Value = Null();

		GetParam(QueryUpdate, "INVOICE_NETTO", ftInteger)->Value =
			Van->InvoiceNetto;
		GetParam(QueryUpdate, "INVOICE_TARE", ftInteger)->Value =
			Van->InvoiceTare;

#ifdef SQL_TO_LOG
		WriteToLog("UPDATE PARAMS: ID = " + VarToStr(Van->ID) + ", " +
			"INVNUM = " + VarToStr(Van->VanNum) + ", ...");
#endif

		QueryUpdate->ExecSQL();

#ifdef SQL_TO_LOG
		WriteToLog("RowsAffected: " + IntToStr(QueryUpdate->RowsAffected));
#endif

		if (QueryUpdate->RowsAffected != 1) {
			throw Exception("record with id=" + IntToStr(Van->ID) +
				" not exists");
		}
	}
	__finally {
		QueryUpdate->Free();

		Connection->Close();
	}
}

// ---------------------------------------------------------------------------
