// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsStr.h>

#include "NVansStrings.h"

#include "NVansTConnectionInfo.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TConnectionInfo::TConnectionInfo() {
	FUser = "root";
	FPassword = "";
}

// ---------------------------------------------------------------------------
