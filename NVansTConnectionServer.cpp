// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsStr.h>

#include "NVansStrings.h"

#include "NVansTConnectionServer.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TConnectionServer::TConnectionServer() : TConnection() {
	FDriver = "";
	FHost = "localhost";
	FPort = "";
}

// ---------------------------------------------------------------------------
