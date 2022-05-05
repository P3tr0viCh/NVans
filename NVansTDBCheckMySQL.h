// ---------------------------------------------------------------------------

#ifndef NVansTDBCheckMySQLH
#define NVansTDBCheckMySQLH

#include "NVansTDBCheck.h"
#include "NVansTConnectionMySQL.h"

// ---------------------------------------------------------------------------
class TDBCheckMySQL : public TDBCheck {
public:
	__fastcall TDBCheckMySQL(TConnectionMySQL * Connection);

	TField * GetFieldVersion();

	String ConnectionToLogString();
};

// ---------------------------------------------------------------------------
#endif
