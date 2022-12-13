// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsSQL.h>

#include "NVansStrings.h"

#include "NVansKeyOracleTrain.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TKeyOracleTrain::TKeyOracleTrain() {
	Init();
}

// ---------------------------------------------------------------------------
void TKeyOracleTrain::Init() {
    FDateTime = DEFAULT_DATETIME;
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

	TKeyOracleTrain * Key = (TKeyOracleTrain*) Obj;

	if (TrainNum != Key->TrainNum || DateTime != Key->DateTime) {
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
