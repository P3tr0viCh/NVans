// ---------------------------------------------------------------------------

#pragma hdrstop

#include "NVansTFilterOracleTrains.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TFilterOracleTrains::TFilterOracleTrains() {
	Init();
}

// ---------------------------------------------------------------------------
void TFilterOracleTrains::Init() {
}

// ---------------------------------------------------------------------------
__fastcall TFilterOracleTrains::~TFilterOracleTrains() {
}

// ---------------------------------------------------------------------------
bool __fastcall TFilterOracleTrains::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TFilterOracleTrains * FilterOracleTrains = (TFilterOracleTrains*) Obj;

	if (Date != FilterOracleTrains->Date ||
		VanNum != FilterOracleTrains->VanNum) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TFilterOracleTrains::Assign(TFilterOracleTrains * Source) {
	Date = Source->Date;
	VanNum = Source->VanNum;
}

// ---------------------------------------------------------------------------
String __fastcall TFilterOracleTrains::ToString() {
	String S;

	S = "TFilterOracleTrains{";
	S += "Date=" + DateToStr(Date);
	S += ",";
	S += "VanNum='" + VanNum + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
