// ---------------------------------------------------------------------------

#ifndef NVansTConnectionOracleH
#define NVansTConnectionOracleH

#include <System.Classes.hpp>

#include "NVansTConnectionServer.h"

// ---------------------------------------------------------------------------
class TConnectionOracle : public TConnectionServer {
private:
	String FService;

	String GetConnectionString();

public:
	__fastcall TConnectionOracle();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TConnectionInfo * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property String Service = {read = FService, write = FService};
};

// ---------------------------------------------------------------------------
#endif
