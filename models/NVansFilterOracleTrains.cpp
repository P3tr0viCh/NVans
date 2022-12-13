// ---------------------------------------------------------------------------

#pragma hdrstop

#include "NVansFilterOracleTrains.h"

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

	if (Date != FilterOracleTrains->Date)
		return false;
	if (VanNum != FilterOracleTrains->VanNum)
		return false;
	if (InvoiceNum_1 != FilterOracleTrains->InvoiceNum_1)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TFilterOracleTrains::Assign(TFilterOracleTrains * Source) {
	Date = Source->Date;
	VanNum = Source->VanNum;
	InvoiceNum_1 = Source->InvoiceNum_1;
}

// ---------------------------------------------------------------------------
String __fastcall TFilterOracleTrains::ToString() {
	String S;

	S = "TFilterOracleTrains{";
	S += "Date=" + DateToStr(Date);
	S += ",";
	S += "VanNum='" + VanNum + "'";
	S += ",";
	S += "InvoiceNum_1='" + InvoiceNum_1 + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
