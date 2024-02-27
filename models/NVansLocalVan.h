// ---------------------------------------------------------------------------

#ifndef NVansLocalVanH
#define NVansLocalVanH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <ObjList.h>

enum TTareIndex {
	tiTrafaret = 0, tiDynamic = 1, tiStatic = 2
};

const TTareIndex DEFAULT_TARE_INDEX = tiTrafaret;

// ---------------------------------------------------------------------------
class TLocalVan : public TObject {
private:
	int FID;

	int FScaleNum;

	int FNum;

	TDateTime FDateTime;

	String FVanNum;

	String FCargoType;
	int FCargoTypeCode;

	String FInvoiceNum;
	String FInvoiceSupplier;
	String FInvoiceRecipient;
	String FDepartStation;
	String FPurposeStation;

	int FCarrying;

	int FBrutto;

	int FTare;
	int FTareT;

	TTareIndex FTareIndex;

	int FTareScaleNum;
	TDateTime FTareDateTime;

	int FNetto;

	int FInvoiceNetto;
	int FInvoiceTare;

	int FNettoDiff;
	int FOverload;

	bool FLoaded;

	bool FCalcFields;

	// -----------------------------------------------------------------------
	void Init();

	void UpdateValues();

	void SetCarrying(int Value);

	void SetInvoiceNetto(int Value);

	void SetBrutto(int Value);
	void SetNetto(int Value);

	void SetTareT(int Value);

	void SetTareIndex(TTareIndex Value);

	void SetCalcFields(bool Value);

public:
	__fastcall TLocalVan();
	__fastcall ~TLocalVan();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TLocalVan * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property int ID = {read = FID, write = FID};

	__property int ScaleNum = {read = FScaleNum, write = FScaleNum};

	__property int Num = {read = FNum, write = FNum};

	__property TDateTime DateTime = {read = FDateTime, write = FDateTime};

	__property String VanNum = {read = FVanNum, write = FVanNum};

	__property String CargoType = {read = FCargoType, write = FCargoType};
	__property int CargoTypeCode = {read = FCargoTypeCode, write = FCargoTypeCode};

	__property String InvoiceNum = {read = FInvoiceNum, write = FInvoiceNum};

	__property String InvoiceSupplier = {
		read = FInvoiceSupplier, write = FInvoiceSupplier};
	__property String InvoiceRecipient = {
		read = FInvoiceRecipient, write = FInvoiceRecipient};

	__property String DepartStation = {
		read = FDepartStation, write = FDepartStation};
	__property String PurposeStation = {
		read = FPurposeStation, write = FPurposeStation};

	__property int Carrying = {read = FCarrying, write = SetCarrying};

	__property int Brutto = {read = FBrutto, write = SetBrutto};

	__property int Tare = {read = FTare, write = FTare};
	__property int TareT = {read = FTareT, write = SetTareT};

	__property TTareIndex TareIndex = {read = FTareIndex, write = SetTareIndex};

	__property int TareScaleNum = {read = FTareScaleNum, write = FTareScaleNum};
	__property TDateTime TareDateTime = {
		read = FTareDateTime, write = FTareDateTime};

	__property int Netto = {read = FNetto, write = SetNetto};

	__property int InvoiceNetto = {read = FInvoiceNetto, write = SetInvoiceNetto
	};
	__property int InvoiceTare = {read = FInvoiceTare, write = FInvoiceTare};

	__property int NettoDiff = {read = FNettoDiff};
	__property int Overload = {read = FOverload};

	__property bool IsLoaded = {read = FLoaded, write = FLoaded};

	__property bool CalcFields = {read = FCalcFields, write = SetCalcFields};
};

// ---------------------------------------------------------------------------
typedef TObjList<TLocalVan>TLocalVanList;

// ---------------------------------------------------------------------------
#endif
