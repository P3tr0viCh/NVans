// ---------------------------------------------------------------------------

#ifndef NVansTDBLocalSaveVanH
#define NVansTDBLocalSaveVanH

#include "NVansTVan.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBLocalSaveVan : public TDatabaseOperation {
private:
	TVan * FVan;

public:
	__fastcall TDBLocalSaveVan(TConnectionInfo * ConnectionInfo);
	__fastcall ~TDBLocalSaveVan();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	void SetVan(TVan * Van);

	// -----------------------------------------------------------------------
	__property TVan * Van = {read = FVan, write = SetVan};
};

// ---------------------------------------------------------------------------
#endif
