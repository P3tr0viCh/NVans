// ---------------------------------------------------------------------------

#pragma hdrstop

#include "NVansStrings.h"

#include "NVansTVan.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TVan::TVan() {
	Init();
}

// ---------------------------------------------------------------------------
void TVan::Init() {
	FID = -1;

	FNum = 0;

	FCarrying = 0;
	FTareT = 0;
	FInvoiceNetto = 0;
	FInvoiceTare = 0;

	FLoaded = false;

	FBrutto = 0;
	FNetto = 0;
}

// ---------------------------------------------------------------------------
__fastcall TVan::~TVan() {
}

// ---------------------------------------------------------------------------
bool __fastcall TVan::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TVan * Van = (TVan*) Obj;

	if (ID != Van->ID || Num != Van->Num || DateTime != Van->DateTime ||
		VanNum != Van->VanNum || CargoType != Van->CargoType ||
		InvoiceNum != Van->InvoiceNum ||
		InvoiceSupplier != Van->InvoiceSupplier ||
		InvoiceRecipient != Van->InvoiceRecipient ||
		DepartStation != Van->DepartStation ||
		PurposeStation != Van->PurposeStation || Carrying != Van->Carrying ||
		TareT != Van->TareT || InvoiceNetto != Van->InvoiceNetto ||
		InvoiceTare != Van->InvoiceTare || IsLoaded != Van->IsLoaded ||
		Brutto != Van->Brutto || Netto != Van->Netto) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TVan::Assign(TVan * Source) {
	ID = Source->ID;

	Num = Source->Num;

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

	IsLoaded = Source->IsLoaded;

	Brutto = Source->Brutto;
	Netto = Source->Netto;
}

// ---------------------------------------------------------------------------
String __fastcall TVan::ToString() {
	String S;

	S = "TVan{";
	S += "ID=" + IntToStr(ID);
	S += ",";
	S += "Num=" + IntToStr(Num);
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
	S += ",";
	S += "IsLoaded='" + BoolToStr(IsLoaded, true) + "'";
	S += ",";
	S += "Brutto='" + IntToStr(Brutto) + "'";
	S += ",";
	S += "Netto='" + IntToStr(Netto) + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
