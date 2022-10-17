// ---------------------------------------------------------------------------

#ifndef NVansTDBCheckMySQLH
#define NVansTDBCheckMySQLH

#include <ConnectionMySQL.h>

#include "NVansTDBCheck.h"

// ---------------------------------------------------------------------------
class TDBCheckMySQL : public TDBCheck {
public:
	__fastcall TDBCheckMySQL(TConnectionMySQL * Connection);

	TField * GetFieldVersion();

	String ConnectionToLogString();
};

// ---------------------------------------------------------------------------
#endif
