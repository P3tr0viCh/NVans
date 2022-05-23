// ---------------------------------------------------------------------------

#ifndef NVansTDBLocalSaveVanH
#define NVansTDBLocalSaveVanH

#include "NVansTLocalVan.h"

#include "NVansTDatabaseOperation.h"

// ---------------------------------------------------------------------------
class TDBLocalSaveVan : public TDatabaseOperation {
private:
	TLocalVan * FVan;

public:
	__fastcall TDBLocalSaveVan(TConnectionInfo * ConnectionInfo);
	__fastcall ~TDBLocalSaveVan();

	void OperationStart();
	void OperationEndOK();
	void OperationEndFail();

	void Operation();

	void SetVan(TLocalVan * Van);

	// -----------------------------------------------------------------------
	__property TLocalVan * Van = {read = FVan, write = SetVan};
};

// ---------------------------------------------------------------------------
#endif
