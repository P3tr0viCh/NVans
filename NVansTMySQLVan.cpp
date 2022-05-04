// ---------------------------------------------------------------------------

#pragma hdrstop

#include "NVansStrings.h"

#include "NVansTMySQLVan.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TMySQLVan::TMySQLVan() {
	Init();
}

// ---------------------------------------------------------------------------
void TMySQLVan::Init() {
	FNum = 0;

	FCarrying = 0;
	FTareT = 0;
	FInvoiceNetto = 0;
	FInvoiceTare = 0;
}

// ---------------------------------------------------------------------------
__fastcall TMySQLVan::~TMySQLVan() {
}

// ---------------------------------------------------------------------------
bool __fastcall TMySQLVan::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TMySQLVan * Van = (TMySQLVan*) Obj;

	if (Num != Van->Num || VanNum != Van->VanNum ||
		CargoType != Van->CargoType || InvoiceNum != Van->InvoiceNum ||
		InvoiceSupplier != Van->InvoiceSupplier ||
		InvoiceRecipient != Van->InvoiceRecipient ||
		DepartStation != Van->DepartStation ||
		PurposeStation != Van->PurposeStation || Carrying != Van->Carrying ||
		TareT != Van->TareT || InvoiceNetto != Van->InvoiceNetto ||
		InvoiceTare != Van->InvoiceTare) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TMySQLVan::Assign(TMySQLVan * Source) {
	Num = Source->Num;

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
String __fastcall TMySQLVan::ToString() {
	String S;

	S = "TMySQLVan{";
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
	S += ",";
	S += "Carrying='" + IntToStr(Carrying) + "'";
	S += ",";
	S += "TareT='" + IntToStr(TareT) + "'";
	S += ",";
	S += "InvoiceNetto='" + IntToStr(InvoiceNetto) + "'";
	S += ",";
	S += "InvoiceTare='" + IntToStr(InvoiceTare) + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
