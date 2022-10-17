// ---------------------------------------------------------------------------

#ifndef NVansTDBCheckOracleH
#define NVansTDBCheckOracleH

#include <ConnectionOracle.h>

#include "NVansTDBCheck.h"

// ---------------------------------------------------------------------------
class TDBCheckOracle : public TDBCheck {
public:
	__fastcall TDBCheckOracle(TConnectionOracle * Connection);

	TField * GetFieldVersion();

	String ConnectionToLogString();
};

// ---------------------------------------------------------------------------
#endif
