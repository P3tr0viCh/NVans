// ---------------------------------------------------------------------------

#ifndef NVansDBIsvsLoadCargoTypesH
#define NVansDBIsvsLoadCargoTypesH

#include <CodeNamePair.h>

#include <DBOperation.h>

// ---------------------------------------------------------------------------
class TDBIsvsLoadCargoTypes : public TDBOperation {
private:
	TCodeNamePairList * FCargoTypeList;

public:
	__fastcall TDBIsvsLoadCargoTypes(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent,
		TCodeNamePairList * CargoTypeList);
	__fastcall ~TDBIsvsLoadCargoTypes();

	void Operation();

	// -----------------------------------------------------------------------
	__property TCodeNamePairList * CargoTypeList = {read = FCargoTypeList};
};

// ---------------------------------------------------------------------------
#endif
