//---------------------------------------------------------------------------

#ifndef NVansTSettingsH
#define NVansTSettingsH

#include <System.Classes.hpp>

// ---------------------------------------------------------------------------
class TSettings : public TObject {
private:
	TColor FColorReadOnly;
public:
	__fastcall TSettings();
	__fastcall ~TSettings();

	__property TColor ColorReadOnly = {read = FColorReadOnly};
};

//---------------------------------------------------------------------------
#endif
