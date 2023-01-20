// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsSQL.h>

#include "NVansStrings.h"

#include "NVansOracleVan.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TOracleVan::TOracleVan() {
	Init();
}

// ---------------------------------------------------------------------------
void TOracleVan::Init() {
	FNum = 0;

	FInvoiceDateTime = DEFAULT_DATETIME;

	FCarrying = 0;
	FTareT = 0;
	FInvoiceNetto = 0;
	FInvoiceTare = 0;
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

	if (Num != Van->Num)
		return false;

	if (InvoiceDateTime != Van->InvoiceDateTime)
		return false;

	if (VanNum != Van->VanNum)
		return false;

	if (CargoType != Van->CargoType)
		return false;

	if (InvoiceNum != Van->InvoiceNum)
		return false;

	if (InvoiceSupplier != Van->InvoiceSupplier)
		return false;
	if (InvoiceRecipient != Van->InvoiceRecipient)
		return false;

	if (DepartStation != Van->DepartStation)
		return false;
	if (PurposeStation != Van->PurposeStation)
		return false;

	if (Carrying != Van->Carrying)
		return false;
	if (TareT != Van->TareT)
		return false;

	if (InvoiceNetto != Van->InvoiceNetto)
		return false;
	if (InvoiceTare != Van->InvoiceTare)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TOracleVan::Assign(TOracleVan * Source) {
	Num = Source->Num;

	InvoiceDateTime = Source->InvoiceDateTime;

	VanNum = Source->VanNum;

	CargoType = Source->CargoType;

	InvoiceNum = Source->InvoiceNum;

	InvoiceSupplier = Source->InvoiceSupplier;
	InvoiceRecipient = Source->InvoiceRecipient;

	DepartStation = Source->DepartStation;
	PurposeStation = Source->PurposeStation;

	Carrying = Source->Carrying;
	TareT = Source->TareT;

	InvoiceNetto = Source->InvoiceNetto;
	InvoiceTare = Source->InvoiceTare;
}

// ---------------------------------------------------------------------------
String __fastcall TOracleVan::ToString() {
	String S;

	S = "TOracleVan{";
	S += "Num=" + IntToStr(Num);
	S += ",";
	S += "InvoiceDateTime=" + DateTimeToStr(FInvoiceDateTime);
	S += ",";
	S += "VanNum=" + VanNum;
	S += ",";
	S += "CargoType=" + CargoType;
	S += ",";
	S += "InvoiceNum=" + InvoiceNum;
	S += ",";
	S += "InvoiceSupplier=" + InvoiceSupplier;
	S += ",";
	S += "InvoiceRecipient=" + InvoiceRecipient;
	S += ",";
	S += "DepartStation=" + DepartStation;
	S += ",";
	S += "PurposeStation=" + PurposeStation;
	S += ",";
	S += "Carrying=" + IntToStr(Carrying);
	S += ",";
	S += "TareT=" + IntToStr(TareT);
	S += ",";
	S += "InvoiceNetto=" + IntToStr(InvoiceNetto);
	S += ",";
	S += "InvoiceTare=" + IntToStr(InvoiceTare);
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
