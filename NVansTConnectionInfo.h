// ---------------------------------------------------------------------------

#ifndef NVansTConnectionInfoH
#define NVansTConnectionInfoH

#include <System.Classes.hpp>

// ---------------------------------------------------------------------------
class TConnectionInfo : public TObject {
private:
	String FUser;
	String FPassword;

	virtual String GetConnectionString() = 0;

public:
	__fastcall TConnectionInfo();

	virtual bool __fastcall Equals(TObject * Obj) = 0;
	virtual void __fastcall Assign(TConnectionInfo * Source) = 0;
	virtual String __fastcall ToString() = 0;

	// -----------------------------------------------------------------------
	__property String User = {read = FUser, write = FUser};
	__property String Password = {read = FPassword, write = FPassword};

	__property String ConnectionString = {read = GetConnectionString};
};

// ---------------------------------------------------------------------------
#endif
