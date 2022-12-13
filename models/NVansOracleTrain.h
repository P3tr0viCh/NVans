// ---------------------------------------------------------------------------

#ifndef NVansOracleTrainH
#define NVansOracleTrainH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <ObjList.h>

// ---------------------------------------------------------------------------
class TOracleTrain : public TObject {
private:
	String FTrainNum;
	TDateTime FDateTime;
	int FVanCount;

	// -----------------------------------------------------------------------
	void Init();

public:
	__fastcall TOracleTrain();
	__fastcall ~TOracleTrain();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TOracleTrain * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property String TrainNum = {read = FTrainNum, write = FTrainNum};
	__property TDateTime DateTime = {read = FDateTime, write = FDateTime};
	__property int VanCount = {read = FVanCount, write = FVanCount};
};

// ---------------------------------------------------------------------------
typedef TObjList<TOracleTrain>TOracleTrainList;

// ---------------------------------------------------------------------------
#endif
