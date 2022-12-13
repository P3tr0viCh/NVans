// ---------------------------------------------------------------------------

#ifndef NVansDBLocalSaveVanH
#define NVansDBLocalSaveVanH

#include <DBOperation.h>

#include "NVansLocalVan.h"

// ---------------------------------------------------------------------------
class TDBLocalSaveVan : public TDBOperation {
private:
	TLocalVan * FVan;

	void SetVan(TLocalVan * Van);

public:
	__fastcall TDBLocalSaveVan(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent);
	__fastcall ~TDBLocalSaveVan();

	void Operation();

	// -----------------------------------------------------------------------
	__property TLocalVan * Van = {read = FVan, write = SetVan};
};

// ---------------------------------------------------------------------------
#endif
