// ---------------------------------------------------------------------------

#ifndef NVansDBIsvsLoadStationsH
#define NVansDBIsvsLoadStationsH

#include <CodeNamePair.h>

#include <DBOperation.h>

// ---------------------------------------------------------------------------
class TDBIsvsLoadStations : public TDBOperation {
private:
	TCodeNamePairList * FStationList;

public:
	__fastcall TDBIsvsLoadStations(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent, TCodeNamePairList * StationList);
	__fastcall ~TDBIsvsLoadStations();

	void Operation();

	// -----------------------------------------------------------------------
	__property TCodeNamePairList * StationList = {read = FStationList};
};

// ---------------------------------------------------------------------------
#endif
