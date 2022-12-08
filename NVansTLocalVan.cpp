// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsSQL.h>

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

	FScaleNum = 0;

	FNum = 0;

	FDateTime = DEFAULT_DATETIME;

	FCarrying = 0;
	FTareT = 0;
	FInvoiceNetto = 0;
	FInvoiceTare = 0;

	FLoaded = false;

	FBrutto = 0;
	FNetto = 0;

	FNettoDiff = 0;

	FTareIndex = DEFAULT_TARE_INDEX;

	FTareScaleNum = 0;
	FTareDateTime = DEFAULT_DATETIME;

	FCalcFields = false;
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

	if (ID != Van->ID)
		return false;

	if (ScaleNum != Van->ScaleNum)
		return false;

	if (Num != Van->Num)
		return false;

	if (DateTime != Van->DateTime)
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

	if (Brutto != Van->Brutto)
		return false;

	if (TareT != Van->TareT)
		return false;

	if (TareIndex != Van->TareIndex)
		return false;

	if (TareScaleNum != Van->TareScaleNum)
		return false;
	if (TareDateTime != Van->TareDateTime)
		return false;

	if (Netto != Van->Netto)
		return false;

	if (InvoiceNetto != Van->InvoiceNetto)
		return false;
	if (InvoiceTare != Van->InvoiceTare)
		return false;

	if (NettoDiff != Van->NettoDiff)
		return false;
	if (Overload != Van->Overload)
		return false;

	if (IsLoaded != Van->IsLoaded)
		return false;

	if (CalcFields != Van->CalcFields)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TLocalVan::Assign(TLocalVan * Source) {
	CalcFields = false;

	ID = Source->ID;

	ScaleNum = Source->ScaleNum;

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

	Brutto = Source->Brutto;

	Tare = Source->Tare;
	TareT = Source->TareT;

	TareIndex = Source->TareIndex;

	TareScaleNum = Source->TareScaleNum;
	TareDateTime = Source->TareDateTime;

	Netto = Source->Netto;

	InvoiceNetto = Source->InvoiceNetto;
	InvoiceTare = Source->InvoiceTare;

	IsLoaded = Source->IsLoaded;

	CalcFields = Source->CalcFields;
}

// ---------------------------------------------------------------------------
String __fastcall TLocalVan::ToString() {
	String S;

	S = "TLocalVan{";
	S += "ID=" + IntToStr(ID);
	S += ",";
	S += "ScaleNum=" + IntToStr(ScaleNum);
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
	S += "Brutto='" + IntToStr(Brutto) + "'";
	S += ",";
	S += "Tare='" + IntToStr(Tare) + "'";
	S += ",";
	S += "TareT='" + IntToStr(TareT) + "'";
	S += ",";
	S += "TareIndex='" + IntToStr(TareIndex) + "'";
	S += ",";
	S += "TareScaleNum='" + IntToStr(TareScaleNum) + "'";
	S += ",";
	S += "TareDateTime='" + DateTimeToStr(TareDateTime) + "'";
	S += ",";
	S += "Netto='" + IntToStr(Netto) + "'";
	S += ",";
	S += "InvoiceNetto='" + IntToStr(InvoiceNetto) + "'";
	S += ",";
	S += "InvoiceTare='" + IntToStr(InvoiceTare) + "'";
	S += ",";
	S += "NettoDiff='" + IntToStr(NettoDiff) + "'";
	S += ",";
	S += "Overload='" + IntToStr(Overload) + "'";
	S += ",";
	S += "IsLoaded='" + BoolToStr(IsLoaded, true) + "'";
	S += ",";
	S += "CalcFields='" + BoolToStr(CalcFields, true) + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
void TLocalVan::UpdateValues() {
	if (!CalcFields) {
		FOverload = 0;
		FNettoDiff = 0;

		return;
	}

	if (FBrutto > 0 && FTare > 0) {
		FNetto = FBrutto - FTare;
	}
	else {
		FNetto = 0;
	}

	if (FCarrying > 0 && FNetto > 0) {
		FOverload = FNetto - FCarrying;
	}
	else {
		FOverload = 0;
	}

	if (FNetto > 0 && FInvoiceNetto > 0) {
		FNettoDiff = FNetto - FInvoiceNetto;
	}
	else {
		FNettoDiff = 0;
	}
}

// ---------------------------------------------------------------------------
void TLocalVan::SetCarrying(int Value) {
	if (FCarrying == Value) {
		return;
	}

	FCarrying = Value;

	UpdateValues();
}

// ---------------------------------------------------------------------------
void TLocalVan::SetInvoiceNetto(int Value) {
	if (FInvoiceNetto == Value) {
		return;
	}

	FInvoiceNetto = Value;

	UpdateValues();
}

// ---------------------------------------------------------------------------
void TLocalVan::SetBrutto(int Value) {
	if (FBrutto == Value) {
		return;
	}

	FBrutto = Value;

	UpdateValues();
}

// ---------------------------------------------------------------------------
void TLocalVan::SetNetto(int Value) {
	if (FNetto == Value) {
		return;
	}

	FNetto = Value;

	UpdateValues();
}

// ---------------------------------------------------------------------------
void TLocalVan::SetTareT(int Value) {
	if (FTareT == Value) {
		return;
	}

	FTareT = Value;

	UpdateValues();
}

// ---------------------------------------------------------------------------
void TLocalVan::SetTareIndex(TTareIndex Value) {
	if (FTareIndex == Value) {
		return;
	}

	FTareIndex = Value;

	UpdateValues();
}

// ---------------------------------------------------------------------------
void TLocalVan::SetCalcFields(bool Value) {
	if (FCalcFields == Value) {
		return;
	}

	FCalcFields = Value;

	UpdateValues();
}

// ---------------------------------------------------------------------------
