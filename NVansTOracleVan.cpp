// ---------------------------------------------------------------------------

#pragma hdrstop

#include "NVansStrings.h"

#include "NVansTOracleVan.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TOracleVan::TOracleVan() {
	Init();
}

// ---------------------------------------------------------------------------
void TOracleVan::Init() {
	FNum = 0;
}

// ---------------------------------------------------------------------------
__fastcall TOracleVan::~TOracleVan() {
}

// ---------------------------------------------------------------------------
bool __fastcall TOracleVan::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TOracleVan * Van = (TOracleVan*) Obj;

	if (Num != Van->Num || VanNum != Van->VanNum ||
		CargoType != Van->CargoType || InvoiceNum != Van->InvoiceNum ||
		InvoiceSupplier != Van->InvoiceSupplier ||
		InvoiceRecipient != Van->InvoiceRecipient ||
		DepartStation != Van->DepartStation ||
		PurposeStation != Van->PurposeStation) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TOracleVan::Assign(TOracleVan * Source) {
	Num = Source->Num;

	VanNum = Source->VanNum;

	CargoType = Source->CargoType;

	InvoiceNum = Source->InvoiceNum;

	InvoiceSupplier = Source->InvoiceSupplier;
	InvoiceRecipient = Source->InvoiceRecipient;

	DepartStation = Source->DepartStation;
	PurposeStation = Source->PurposeStation;
}

// ---------------------------------------------------------------------------
String __fastcall TOracleVan::ToString() {
	String S;

	S = "TOracleVan{";
	S += "Num=" + IntToStr(Num);
	S += ",";
	S += "VanNum=" + VanNum;
	S += ",";
	S += "CargoType='" + CargoType + "'";
	S += ",";
	S += "InvoiceNum='" + InvoiceNum + "'";
	S += ",";
	S += "InvoiceSupplier='" + InvoiceSupplier + "'";
	S += ",";
	S += "InvoiceRecipient='" + InvoiceRecipient + "'";
	S += ",";
	S += "DepartStation='" + DepartStation + "'";
	S += ",";
	S += "PurposeStation='" + PurposeStation + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
