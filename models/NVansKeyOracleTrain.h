// ---------------------------------------------------------------------------

#ifndef NVansKeyOracleTrainH
#define NVansKeyOracleTrainH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <ObjList.h>

// ---------------------------------------------------------------------------
class TKeyOracleTrain : public TObject {
private:
	String FTrainNum;
	TDateTime FDateTime;

	// -----------------------------------------------------------------------
	void Init();

public:
	__fastcall TKeyOracleTrain();
	__fastcall ~TKeyOracleTrain();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TKeyOracleTrain * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property String TrainNum = {read = FTrainNum, write = FTrainNum};
	__property TDateTime DateTime = {read = FDateTime, write = FDateTime};
};

// ---------------------------------------------------------------------------
typedef TObjList<TKeyOracleTrain>TKeyOracleTrainList;

// ---------------------------------------------------------------------------
#endif
