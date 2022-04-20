// ---------------------------------------------------------------------------

#pragma hdrstop

#include "NVansStrings.h"

#include "NVansTOracleTrain.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TOracleTrain::TOracleTrain() {
	Init();
}

// ---------------------------------------------------------------------------
void TOracleTrain::Init() {
}

// ---------------------------------------------------------------------------
__fastcall TOracleTrain::~TOracleTrain() {
}

// ---------------------------------------------------------------------------
bool __fastcall TOracleTrain::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TOracleTrain * Train = (TOracleTrain*) Obj;

	if (TrainNum != Train->TrainNum || DateTime != Train->DateTime ||
		VanCount != Train->VanCount) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TOracleTrain::Assign(TOracleTrain * Source) {
	TrainNum = Source->TrainNum;
	DateTime = Source->DateTime;
	VanCount = Source->VanCount;
}

// ---------------------------------------------------------------------------
String __fastcall TOracleTrain::ToString() {
	String S;

	S = "TOracleTrain{";
	S += "TrainNum=" + TrainNum;
	S += ",";
	S += "DateTime=" + DateTimeToStr(DateTime);
	S += ",";
	S += "VanCount='" + IntToStr(VanCount) + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
