// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>

#include "NVansStrings.h"
#include "NVansStringsSQL.h"

#include "NVansTDBCheckMySQL.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBCheckMySQL::TDBCheckMySQL(TConnectionMySQL * Connection)
	: TDBCheck(Connection) {
	FQuery = LoadStr(IDS_SQL_MYSQL_VERSION);
}

// ---------------------------------------------------------------------------
TField * TDBCheckMySQL::GetFieldVersion() {
	return DataSet->Fields->Fields[0];
}

// ---------------------------------------------------------------------------
String TDBCheckMySQL::ConnectionToLogString() {
	return Format(IDS_LOG_DATABASE_CONNECT,
		ARRAYOFCONST((ConnectionInfo->User,
		((TConnectionMySQL*)ConnectionInfo)->Host,
		((TConnectionMySQL*)ConnectionInfo)->Port,
		((TConnectionMySQL*)ConnectionInfo)->Database)));
}

// ---------------------------------------------------------------------------
