// ---------------------------------------------------------------------------

#ifndef NVansTConnectionMySQLH
#define NVansTConnectionMySQLH

#include <System.Classes.hpp>

#include "NVansTConnectionServer.h"

// ---------------------------------------------------------------------------
class TConnectionMySQL : public TConnectionServer {
private:
	String FDatabase;

	// -----------------------------------------------------------------------
	String GetConnectionString();

public:
	__fastcall TConnectionMySQL();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TConnectionInfo * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property String Database = {read = FDatabase, write = FDatabase};
};

// ---------------------------------------------------------------------------
#endif
