// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsSQL.h>

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
	FVan = new TLocalVan();
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
void TDBLocalSaveVan::SetVan(TLocalVan * Van) {
	FVan->Assign(Van);
}

// ---------------------------------------------------------------------------
void TDBLocalSaveVan::Operation() {
	Connection->Open();

	TADOQuery * QueryUpdate = new TADOQuery(NULL);
	try {
		QueryUpdate->Connection = Connection;

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

		SQLGetParam(QueryUpdate, "ID", ftInteger)->Value = Van->ID;

		SQLGetParam(QueryUpdate, "INVNUM", ftString)->Value = Van->VanNum;

		SQLGetParam(QueryUpdate, "CARRYING", ftInteger)->Value = Van->Carrying;
		SQLGetParam(QueryUpdate, "LOADNORM", ftInteger)->Value = Van->Carrying;

		SQLGetParam(QueryUpdate, "TARE", ftInteger)->Value = Van->Tare;
		SQLGetParam(QueryUpdate, "TARE_T", ftInteger)->Value = Van->TareT;
		SQLGetParam(QueryUpdate, "ISCALES_TARE", ftInteger)->Value =
			Van->TareScaleNum;
		SQLGetParam(QueryUpdate, "IDATETIME_TARE", ftString)->Value =
			DateTimeToSQLStr(Van->TareDateTime);

		SQLGetParam(QueryUpdate, "NETTO", ftInteger)->Value = Van->Netto;

		SQLGetParam(QueryUpdate, "OVERLOAD", ftInteger)->Value = Van->Overload;

		SQLGetParam(QueryUpdate, "CARGOTYPE", ftString)->Value = Van->CargoType;
		SQLGetParam(QueryUpdate, "CARGOTYPE_CODE", ftString)->Value = Null();

		SQLGetParam(QueryUpdate, "DEPART_STATION", ftString)->Value =
			Van->DepartStation;
		SQLGetParam(QueryUpdate, "DEPART_STATION_CODE", ftString)
			->Value = Null();
		SQLGetParam(QueryUpdate, "PURPOSE_STATION", ftString)->Value =
			Van->PurposeStation;
		SQLGetParam(QueryUpdate, "PURPOSE_STATION_CODE", ftString)
			->Value = Null();

		SQLGetParam(QueryUpdate, "INVOICE_NUM", ftString)->Value =
			Van->InvoiceNum;

		SQLGetParam(QueryUpdate, "INVOICE_SUPPLIER", ftString)->Value =
			Van->InvoiceSupplier;
		SQLGetParam(QueryUpdate, "INVOICE_SUPPLIER_CODE", ftString)
			->Value = Null();
		SQLGetParam(QueryUpdate, "INVOICE_CONSIGN", ftString)->Value =
			Van->InvoiceRecipient;
		SQLGetParam(QueryUpdate, "INVOICE_CONSIGN_CODE", ftString)
			->Value = Null();

		SQLGetParam(QueryUpdate, "INVOICE_NETTO", ftInteger)->Value =
			Van->InvoiceNetto;
		SQLGetParam(QueryUpdate, "INVOICE_TARE", ftInteger)->Value =
			Van->InvoiceTare;

		// TODO
		SQLGetParam(QueryUpdate, "DISBALONG", ftInteger)->Value = 0;
		SQLGetParam(QueryUpdate, "DISBCROSS", ftInteger)->Value = 0;
		SQLGetParam(QueryUpdate, "TOL_DISBALONG", ftInteger)->Value = 0;
		SQLGetParam(QueryUpdate, "TOL_DISBCROSS", ftInteger)->Value = 0;

#ifdef SQL_TO_LOG
		WriteToLog("UPDATE PARAMS: ID = " + IntToStr(Van->ID) + ", " +
			"INVNUM = " + Van->VanNum + ", ...");
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
