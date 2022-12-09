// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansTDBCheckOracle.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBCheckOracle::TDBCheckOracle(TConnectionOracle * Connection)
	: TDBCheck(Connection) {
	FQuery = SQLLoad(IDS_SQL_ORACLE_VERSION);
}

// ---------------------------------------------------------------------------
TField * TDBCheckOracle::GetFieldVersion() {
	TField * F = DataSet->Fields->FindField("BANNER_FULL");
	if (F == NULL) {
		F = DataSet->Fields->Fields[0];
	}

	return F;
}

// ---------------------------------------------------------------------------
String TDBCheckOracle::ConnectionToLogString() {
	return Format(IDS_LOG_DATABASE_CONNECT,
		ARRAYOFCONST((ConnectionInfo->User,
		((TConnectionOracle*)ConnectionInfo)->Host,
		((TConnectionOracle*)ConnectionInfo)->Port,
		((TConnectionOracle*)ConnectionInfo)->Service)));
}

// ---------------------------------------------------------------------------
