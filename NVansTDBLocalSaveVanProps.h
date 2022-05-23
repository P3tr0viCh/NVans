// ---------------------------------------------------------------------------

#ifndef NVansTDBLocalSaveVanPropsH
#define NVansTDBLocalSaveVanPropsH

#include "NVansTOracleVan.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBLocalSaveVanProps : public TDatabaseOperation {
private:
	int InsertCount;
	int UpdateCount;

	TOracleVanList * FVanList;

public:
	__fastcall TDBLocalSaveVanProps(TConnectionInfo * ConnectionInfo,
		TOracleVanList * VanList);
	__fastcall ~TDBLocalSaveVanProps();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property TOracleVanList * VanList = {read = FVanList};
};

// ---------------------------------------------------------------------------
#endif
