// ---------------------------------------------------------------------------

#ifndef NVansTConnectionH
#define NVansTConnectionH

#include <System.Classes.hpp>

// ---------------------------------------------------------------------------
class TConnection : public TObject {
private:
	String FUser;
	String FPassword;

	virtual String GetConnectionString() = 0;

public:
	__fastcall TConnection();

	virtual bool __fastcall Equals(TObject * Obj) = 0;
	virtual void __fastcall Assign(TConnection * Source) = 0;
	virtual String __fastcall ToString() = 0;

	// -----------------------------------------------------------------------
	__property String User = {read = FUser, write = FUser};
	__property String Password = {read = FPassword, write = FPassword};

	__property String ConnectionString = {read = GetConnectionString};
};

// ---------------------------------------------------------------------------
#endif
