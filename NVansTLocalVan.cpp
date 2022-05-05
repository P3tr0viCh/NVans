// ---------------------------------------------------------------------------

#pragma hdrstop

#include "NVansStrings.h"

#include "NVansTLocalVan.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TLocalVan::TLocalVan() {
	Init();
}

// ---------------------------------------------------------------------------
void TLocalVan::Init() {
	FID = -1;

	FCarrying = 0;
	FTareT = 0;
	FInvoiceNetto = 0;
	FInvoiceTare = 0;
}

// ---------------------------------------------------------------------------
__fastcall TLocalVan::~TLocalVan() {
}

// ---------------------------------------------------------------------------
bool __fastcall TLocalVan::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TLocalVan * Van = (TLocalVan*) Obj;

	if (ID != Van->ID || DateTime != Van->DateTime || VanNum != Van->VanNum ||
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
void __fastcall TLocalVan::Assign(TLocalVan * Source) {
	ID = Source->ID;

	DateTime = Source->DateTime;

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
String __fastcall TLocalVan::ToString() {
	String S;

	S = "TLocalVan{";
	S += "ID=" + IntToStr(ID);
	S += ",";
	S += "DateTime=" + DateTimeToStr(DateTime);
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
