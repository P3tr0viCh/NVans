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
	FQuery = LoadStr(IDS_SQL_ORACLE_VERSION);
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
