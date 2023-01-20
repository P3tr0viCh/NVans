// ---------------------------------------------------------------------------

#pragma hdrstop

#include "NVansFilterLocalTrains.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TFilterLocalTrains::TFilterLocalTrains() {
	Init();
}

// ---------------------------------------------------------------------------
void TFilterLocalTrains::Init() {
}

// ---------------------------------------------------------------------------
__fastcall TFilterLocalTrains::~TFilterLocalTrains() {
}

// ---------------------------------------------------------------------------
bool __fastcall TFilterLocalTrains::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TFilterLocalTrains * FilterOracleTrains = (TFilterLocalTrains*) Obj;

	if (Date != FilterOracleTrains->Date ||
		VanNum != FilterOracleTrains->VanNum) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TFilterLocalTrains::Assign(TFilterLocalTrains * Source) {
	Date = Source->Date;
	VanNum = Source->VanNum;
}

// ---------------------------------------------------------------------------
String __fastcall TFilterLocalTrains::ToString() {
	String S;

	S = "TFilterLocalTrains{";
	S += "Date=" + DateToStr(Date);
	S += ",";
	S += "VanNum=" + VanNum;
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
