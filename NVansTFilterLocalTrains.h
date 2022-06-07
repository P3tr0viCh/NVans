// ---------------------------------------------------------------------------

#ifndef NVansTFilterLocalTrainsH
#define NVansTFilterLocalTrainsH

#include <System.Classes.hpp>

// ---------------------------------------------------------------------------
class TFilterLocalTrains : public TObject {
private:
	TDate FDate;
	String FVanNum;

	// -----------------------------------------------------------------------
	void Init();

public:
	__fastcall TFilterLocalTrains();
	__fastcall ~TFilterLocalTrains();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TFilterLocalTrains * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property TDate Date = {read = FDate, write = FDate};
	__property String VanNum = {read = FVanNum, write = FVanNum};
};

// ---------------------------------------------------------------------------
#endif
