// ---------------------------------------------------------------------------

#pragma hdrstop

#include "NVansStrings.h"

#include "NVansTKeyOracleTrain.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TKeyOracleTrain::TKeyOracleTrain() {
	Init();
}

// ---------------------------------------------------------------------------
void TKeyOracleTrain::Init() {
}

// ---------------------------------------------------------------------------
__fastcall TKeyOracleTrain::~TKeyOracleTrain() {
}

// ---------------------------------------------------------------------------
bool __fastcall TKeyOracleTrain::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TKeyOracleTrain * Train = (TKeyOracleTrain*) Obj;

	if (TrainNum != Train->TrainNum || DateTime != Train->DateTime) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TKeyOracleTrain::Assign(TKeyOracleTrain * Source) {
	TrainNum = Source->TrainNum;
	DateTime = Source->DateTime;
}

// ---------------------------------------------------------------------------
String __fastcall TKeyOracleTrain::ToString() {
	String S;

	S = "TKeyOracleTrain{";
	S += "TrainNum=" + TrainNum;
	S += ",";
	S += "DateTime=" + DateTimeToStr(DateTime);
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
