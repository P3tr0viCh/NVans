// ---------------------------------------------------------------------------

#ifndef NVansTSettingsH
#define NVansTSettingsH

#include <System.Classes.hpp>

#include "NVansTConnectionMySQL.h"
#include "NVansTConnectionOracle.h"

#define CFG_EXT ".cfg"

// ---------------------------------------------------------------------------
class TSettings : public TObject {
private:
	// При добавлении настройки, обновлять TSettings, Equals, Assign, ToString;

	String FOptionsPass;

	String FConfigFileName;

	TColor FColorReadOnly;

	TConnectionMySQL * FLocalConnection;
	TConnectionMySQL * FServerMySQLConnection;
	TConnectionOracle * FServerOracleConnection;

	// -----------------------------------------------------------------------
	String GetConfigFileName();

	String CRC(String Text);

	String Encrypt(String Text);
	String Decrypt(String Text);

	String GetCRC();
	void CheckCRC(String CRC);

	void LoadSettings();
	void SaveSettings();

public:
	__fastcall TSettings();
	__fastcall ~TSettings();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TSettings * Source);
	String __fastcall ToString();

	bool Load();
	bool Save();

	// -----------------------------------------------------------------------
	__property String ConfigFileName = {read = FConfigFileName};

	__property String OptionsPass = {read = FOptionsPass, write = FOptionsPass};

	__property TColor ColorReadOnly = {
		read = FColorReadOnly, write = FColorReadOnly};

	__property TConnectionMySQL * LocalConnection = {read = FLocalConnection};
	__property TConnectionMySQL * ServerMySQLConnection = {
		read = FServerMySQLConnection};
	__property TConnectionOracle * ServerOracleConnection = {
		read = FServerOracleConnection};
};

// ---------------------------------------------------------------------------
#endif
