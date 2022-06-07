// ---------------------------------------------------------------------------

#ifndef NVansTLocalTrainH
#define NVansTLocalTrainH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <ObjList.h>

// ---------------------------------------------------------------------------
enum TWeightType {
	wtBrutto = 0, wtTare = 1, wtMixed = 2
};

// ---------------------------------------------------------------------------
class TLocalTrain : public TObject {
private:
	String FTrainNum;

	TDateTime FDateTime;

	int FVanCount;

	TWeightType FWeightType;

	// -----------------------------------------------------------------------
	void Init();

public:
	__fastcall TLocalTrain();
	__fastcall ~TLocalTrain();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TLocalTrain * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property String TrainNum = {read = FTrainNum, write = FTrainNum};

	__property TDateTime DateTime = {read = FDateTime, write = FDateTime};

	__property int VanCount = {read = FVanCount, write = FVanCount};

	__property TWeightType WeightType = {read = FWeightType, write = FWeightType
	};
};

// ---------------------------------------------------------------------------
typedef TObjList<TLocalTrain>TLocalTrainList;

// ---------------------------------------------------------------------------
#endif
