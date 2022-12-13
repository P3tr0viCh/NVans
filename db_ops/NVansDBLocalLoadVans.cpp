// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsSQL.h>
#include <UtilsMisc.h>

#include "NVansAdd.h"

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansDBLocalLoadVans.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#define BRUTTO_BIT 0x08

// ---------------------------------------------------------------------------
__fastcall TDBLocalLoadVans::TDBLocalLoadVans(TDBConnection * DBConnection,
	IDBOperationEvent * DBOperationEvent, TDate Date, String TrainNum)
	: TDBOperation(DBConnection, DBOperationEvent) {
	FVanList = new TLocalVanList();

	FDate = Date;

	FTrainNum = TrainNum;
}

// ---------------------------------------------------------------------------
__fastcall TDBLocalLoadVans::~TDBLocalLoadVans() {
	FVanList->Free();
}

// ---------------------------------------------------------------------------
void TDBLocalLoadVans::Operation() {
	Connection->Open();

	TADOQuery * Query = new TADOQuery(NULL);
	try {
		Query->Connection = Connection;

		bool SearchByDate = TrainNum.IsEmpty();

		if (SearchByDate) {
			Query->SQL->Text = SQLLoad(IDS_SQL_LOCAL_MVANS_SELECT_BY_DT);
		}
		else {
			Query->SQL->Text = SQLLoad(IDS_SQL_LOCAL_MVANS_SELECT_BY_TN);
		}

#ifdef SQL_TO_LOG
		WriteToLog(Query->SQL->Text);
#endif

		if (SearchByDate) {
			SQLGetParam(Query, "DATE_FROM", ftDate)->Value = Date - 1;
			SQLGetParam(Query, "DATE_TO", ftDate)->Value = Date + 1;

#ifdef SQL_TO_LOG
			WriteToLog("PARAMS: DATE_FROM = " + DateToStr(Date - 1) + ", " +
				"DATE_TO = " + DateToStr(Date + 1));
#endif
		}
		else {
			SQLGetParam(Query, "TRAIN_NUM", ftInteger)->Value =
				StrToInt(TrainNum);

#ifdef SQL_TO_LOG
			WriteToLog("PARAMS: TRAIN_NUM = " + TrainNum);
#endif
		}

		Query->Open();

		TLocalVan * Van;

		while (!Query->Eof) {
			ProcMess();
			if (CheckExit()) {
				throw EAbort(IDS_LOG_ERROR_TERMINATED_IN_WORK_PROGRESS);
			}

			Van = new TLocalVan();

			Van->ID = Query->FieldByName("id")->AsInteger;

			Van->ScaleNum = Query->FieldByName("scales")->AsInteger;

			Van->Num = Query->FieldByName("num")->AsInteger;

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

			Van->Brutto = Query->FieldByName("brutto")->AsInteger;

			Van->Tare = Query->FieldByName("tare")->AsInteger;
			Van->TareT = Query->FieldByName("tare_t")->AsInteger;

			Van->TareIndex = (TTareIndex)Query->FieldByName("tareindex")
				->AsInteger;

			Van->TareScaleNum = Query->FieldByName("iscales_tare")->AsInteger;
			Van->TareDateTime = Query->FieldByName("idatetime_tare")
				->AsDateTime;

			Van->Netto = Query->FieldByName("netto")->AsInteger;

			Van->InvoiceNetto = Query->FieldByName("invoice_netto")->AsInteger;
			Van->InvoiceTare = Query->FieldByName("invoice_tare")->AsInteger;

			Van->IsLoaded = Query->FieldByName("status")
				->AsInteger & BRUTTO_BIT;

			Van->CalcFields = true;

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
