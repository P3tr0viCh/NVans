// ---------------------------------------------------------------------------

#ifndef NVansDBLocalSaveVanPropsH
#define NVansDBLocalSaveVanPropsH

#include <DBOperation.h>

#include "NVansOracleVan.h"

// ---------------------------------------------------------------------------
class TDBLocalSaveVanProps : public TDBOperation {
private:
	int FInsertCount;
	int FUpdateCount;

	TOracleVanList * FVanList;

public:
	__fastcall TDBLocalSaveVanProps(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent, TOracleVanList * VanList);
	__fastcall ~TDBLocalSaveVanProps();

	void Operation();

	// -----------------------------------------------------------------------
	__property TOracleVanList * VanList = {read = FVanList};

	__property int InsertCount = {read = FInsertCount};
	__property int UpdateCount = {read = FUpdateCount};
};

// ---------------------------------------------------------------------------
#endif
