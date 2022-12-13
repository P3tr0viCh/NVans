// ---------------------------------------------------------------------------

#ifndef NVansOracleVanH
#define NVansOracleVanH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <ObjList.h>

// ---------------------------------------------------------------------------
class TOracleVan : public TObject {
private:
	int FNum;

	TDateTime FInvoiceDateTime;

	String FVanNum;

	String FCargoType;

	String FInvoiceNum;
	String FInvoiceSupplier;
	String FInvoiceRecipient;
	String FDepartStation;
	String FPurposeStation;

	int FCarrying;
	int FTareT;

	int FInvoiceNetto;
	int FInvoiceTare;

	// -----------------------------------------------------------------------
	void Init();

public:
	__fastcall TOracleVan();
	__fastcall ~TOracleVan();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TOracleVan * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property int Num = {read = FNum, write = FNum};

	__property TDateTime InvoiceDateTime = {
		read = FInvoiceDateTime, write = FInvoiceDateTime};

	__property String VanNum = {read = FVanNum, write = FVanNum};

	__property String CargoType = {read = FCargoType, write = FCargoType};

	__property String InvoiceNum = {read = FInvoiceNum, write = FInvoiceNum};

	__property String InvoiceSupplier = {
		read = FInvoiceSupplier, write = FInvoiceSupplier};
	__property String InvoiceRecipient = {
		read = FInvoiceRecipient, write = FInvoiceRecipient};

	__property String DepartStation = {
		read = FDepartStation, write = FDepartStation};
	__property String PurposeStation = {
		read = FPurposeStation, write = FPurposeStation};

	__property int Carrying = {read = FCarrying, write = FCarrying};
	__property int TareT = {read = FTareT, write = FTareT};

	__property int InvoiceNetto = {read = FInvoiceNetto, write = FInvoiceNetto};
	__property int InvoiceTare = {read = FInvoiceTare, write = FInvoiceTare};
};

// ---------------------------------------------------------------------------
typedef TObjList<TOracleVan>TOracleVanList;

// ---------------------------------------------------------------------------
#endif
