// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansTDBLocalLoadVans.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#define BRUTTO_BIT 0x08

// ---------------------------------------------------------------------------
__fastcall TDBLocalLoadVans::TDBLocalLoadVans(TConnectionInfo * ConnectionInfo,
	TDate Date) : TDatabaseOperation(ConnectionInfo) {
	FVanList = new TVanList();

	FDate = Date;
}

// ---------------------------------------------------------------------------
__fastcall TDBLocalLoadVans::~TDBLocalLoadVans() {
	FVanList->Free();
}

// ---------------------------------------------------------------------------
void TDBLocalLoadVans::OperationStart() {
}

// ---------------------------------------------------------------------------
void TDBLocalLoadVans::OperationEndOK() {
	WriteToLog(Format(IDS_LOG_LOCAL_LOAD_VANS_OK,
		ARRAYOFCONST((VanList->Count))));
}

// ---------------------------------------------------------------------------
void TDBLocalLoadVans::OperationEndFail() {
	WriteToLog(Format(IDS_LOG_LOCAL_LOAD_VANS_FAIL, ErrorMessage));
}

// ---------------------------------------------------------------------------
void TDBLocalLoadVans::Operation() {
	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		String QueryText;

		QueryText = SQLMake(QueryText, IDS_SQL_SELECT);
		QueryText = SQLMake(QueryText, IDS_SQL_LOCAL_MVANS_SELECT);
		QueryText = SQLMake(QueryText, IDS_SQL_FROM);
		QueryText = SQLMake(QueryText, IDS_SQL_LOCAL_MVANS_TABLE);
		QueryText = SQLMake(QueryText, IDS_SQL_WHERE);
		QueryText = SQLMake(QueryText, IDS_SQL_LOCAL_MVANS_WHERE);
		QueryText = SQLMake(QueryText, IDS_SQL_ORDER);
		QueryText = SQLMake(QueryText, IDS_SQL_LOCAL_MVANS_ORDER);

		Query->SQL->Text = QueryText;

#ifdef SQL_TO_LOG
		WriteToLog(Query->SQL->Text);
#endif

		TParameter * Param1 = Query->Parameters->ParamByName("DATE_FROM");
		Param1->DataType = ftDate;
		Param1->Value = Date - 1;
		TParameter * Param2 = Query->Parameters->ParamByName("DATE_TO");
		Param2->DataType = ftDate;
		Param2->Value = Date + 1;

#ifdef SQL_TO_LOG
		WriteToLog("PARAMS: DATE_FROM = " + VarToStr(Param1->Value) + ", " +
			"DATE_TO = " + VarToStr(Param2->Value));
#endif

		Query->Open();

		TVan * Van;

		while (!Query->Eof) {
			ProcMess();
			if (CheckExit()) {
				throw EAbort(IDS_LOG_ERROR_TERMINATED_IN_WORK_PROGRESS);
			}

			Van = new TVan();

			Van->ID = Query->FieldByName("id")->AsInteger;

			Van->DateTime = Query->FieldByName("bdatetime")->AsDateTime;

			Van->VanNum = Query->FieldByName("invnum")->AsString;

			Van->CargoType = Trim(Query->FieldByName("cargotype")->AsString);

			Van->InvoiceNum = Trim(Query->FieldByName("invoice_num")->AsString);

			Van->InvoiceSupplier =
				Trim(Query->FieldByName("invoice_supplier")->AsString);
			Van->InvoiceRecipient =
				Trim(Query->FieldByName("invoice_consign")->AsString);

			Van->DepartStation =
				Trim(Query->FieldByName("depart_station")->AsString);
			Van->PurposeStation =
				Trim(Query->FieldByName("purpose_station")->AsString);

			Van->Carrying = Query->FieldByName("carrying")->AsInteger;
			Van->TareT = Query->FieldByName("tare_t")->AsInteger;
			Van->InvoiceNetto = Query->FieldByName("invoice_netto")->AsInteger;
			Van->InvoiceTare = Query->FieldByName("invoice_tare")->AsInteger;

			Van->IsLoaded = Query->FieldByName("status")
				->AsInteger & BRUTTO_BIT;

			Van->Brutto = Query->FieldByName("brutto")->AsInteger;
			Van->Netto = Query->FieldByName("netto")->AsInteger;

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
