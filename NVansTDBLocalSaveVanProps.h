// ---------------------------------------------------------------------------

#ifndef NVansTDBLocalSaveVanPropsH
#define NVansTDBLocalSaveVanPropsH

#include "NVansTVan.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBLocalSaveVanProps : public TDatabaseOperation {
private:
	int InsertCount;
	int UpdateCount;

	TVanList * FVanList;

public:
	__fastcall TDBLocalSaveVanProps(TConnectionInfo * ConnectionInfo,
		TVanList * VanList);
	__fastcall ~TDBLocalSaveVanProps();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	// -----------------------------------------------------------------------
	__property TVanList * VanList = {read = FVanList};
};

// ---------------------------------------------------------------------------
#endif
