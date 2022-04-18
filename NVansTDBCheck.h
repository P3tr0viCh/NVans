// ---------------------------------------------------------------------------

#ifndef NVansTDBCheckH
#define NVansTDBCheckH

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBCheck : public TDatabaseOperation {
private:
	String FDBVersion;

protected:
	String FQuery;

 	// -----------------------------------------------------------------------
	virtual String ConnectionToLogString() = 0;

public:
	__fastcall TDBCheck(TConnectionInfo * ConnectionInfo);

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property String DBVersion = {read = FDBVersion};
};

// ---------------------------------------------------------------------------
#endif
