// ---------------------------------------------------------------------------

#ifndef NVansTDBCheckOracleH
#define NVansTDBCheckOracleH

#include "NVansTDBCheck.h"
#include "NVansTConnectionOracle.h"

// ---------------------------------------------------------------------------
class TDBCheckOracle : public TDBCheck {
public:
	__fastcall TDBCheckOracle(TConnectionOracle * Connection);

	TField * GetFieldVersion();

	String ConnectionToLogString();
};

// ---------------------------------------------------------------------------
#endif
