// ---------------------------------------------------------------------------

#ifndef NVansTConnectionServerH
#define NVansTConnectionServerH

#include <System.Classes.hpp>

#include "NVansTConnectionInfo.h"

// ---------------------------------------------------------------------------
class TConnectionServer : public TConnectionInfo {
private:
	String FDriver;

	String FHost;
	String FPort;

public:
	__fastcall TConnectionServer();

	// -----------------------------------------------------------------------
	__property String Driver = {read = FDriver, write = FDriver};

	__property String Host = {read = FHost, write = FHost};
	__property String Port = {read = FPort, write = FPort};
};

// ---------------------------------------------------------------------------
#endif
