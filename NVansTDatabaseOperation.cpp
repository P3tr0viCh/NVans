// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>

#include "NVansStrings.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDatabaseOperation::TDatabaseOperation(TConnectionInfo * ConnectionInfo) {
	FConnection = new TADOConnection(NULL);

	FConnectionInfo = ConnectionInfo;
}

// ---------------------------------------------------------------------------
__fastcall TDatabaseOperation::~TDatabaseOperation() {
	FConnection->Free();
}

// ---------------------------------------------------------------------------
bool TDatabaseOperation::Execute() {
	bool Result = false;

	OperationStart();

	FConnection->ConnectionString = FConnectionInfo->ConnectionString;

	try {
		Operation();

		Result = true;
	}
	catch (Exception * E) {
		FErrorMessage = E->Message;
	}

	if (Result) {
		OperationEndOK();
	}
	else {
		if (IsEmpty(FErrorMessage)) {
			FErrorMessage = LoadStr(IDS_ERROR_UNKNOWN);
		}

		OperationEndFail();
	}

	return Result;
}

// ---------------------------------------------------------------------------
