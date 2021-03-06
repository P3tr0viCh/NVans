// ---------------------------------------------------------------------------

#pragma hdrstop

#include "NVansStrings.h"

#include "NVansTLocalTrain.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TLocalTrain::TLocalTrain() {
	Init();
}

// ---------------------------------------------------------------------------
void TLocalTrain::Init() {
	FWeightType = wtBrutto;
}

// ---------------------------------------------------------------------------
__fastcall TLocalTrain::~TLocalTrain() {
}

// ---------------------------------------------------------------------------
bool __fastcall TLocalTrain::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TLocalTrain * Train = (TLocalTrain*) Obj;

	if (TrainNum != Train->TrainNum || DateTime != Train->DateTime ||
		VanCount != Train->VanCount || WeightType != Train->WeightType) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TLocalTrain::Assign(TLocalTrain * Source) {
	TrainNum = Source->TrainNum;
	DateTime = Source->DateTime;
	VanCount = Source->VanCount;
	WeightType = Source->WeightType;
}

// ---------------------------------------------------------------------------
String __fastcall TLocalTrain::ToString() {
	String S;

	S = "TLocalTrain{";
	S += "TrainNum=" + TrainNum;
	S += ",";
	S += "DateTime=" + DateTimeToStr(DateTime);
	S += ",";
	S += "VanCount='" + IntToStr(VanCount) + "'";
	S += ",";
	S += "WeightType='" + IntToStr(WeightType) + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
