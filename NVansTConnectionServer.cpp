// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsStr.h>

#include "NVansStrings.h"

#include "NVansTConnectionServer.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TConnectionServer::TConnectionServer() : TConnectionInfo() {
	FDriver = "";
	FHost = "localhost";
	FPort = "";
}

// ---------------------------------------------------------------------------
