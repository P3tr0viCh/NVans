// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>

#include "NVansStrings.h"

#include "NVansTDBCheck.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBCheck::TDBCheck(TConnectionInfo * ConnectionInfo)
	: TDatabaseOperation(ConnectionInfo) {
}

// ---------------------------------------------------------------------------
void TDBCheck::OperationStart() {
	WriteToLog(ConnectionToLogString());
}

// ---------------------------------------------------------------------------
void TDBCheck::OperationEndOK() {
	WriteToLog(Format(IDS_LOG_DATABASE_CONNECT_OK, DBVersion));
}

// ---------------------------------------------------------------------------
void TDBCheck::OperationEndFail() {
	WriteToLog(Format(IDS_LOG_DATABASE_CONNECT_FAIL, ErrorMessage));
}

// ---------------------------------------------------------------------------
void TDBCheck::Operation() {
	Connection->Open();

	try {
		TADODataSet * DataSet = new TADODataSet(NULL);
		try {
			DataSet->Recordset = Connection->Execute(FQuery);

			TField * F = DataSet->Fields->FieldByName("BANNER_FULL");

			if (F == NULL) {
				FDBVersion = DataSet->Fields->Fields[0]->AsString;
			}
			else {
				FDBVersion = F->AsString;
			}
		}
		__finally {
			DataSet->Free();
		}
	}
	__finally {
		Connection->Close();
	}
}

// ---------------------------------------------------------------------------
