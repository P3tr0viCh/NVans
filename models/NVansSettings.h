// ---------------------------------------------------------------------------

#ifndef NVansSettingsH
#define NVansSettingsH

#include <System.Classes.hpp>

#include <DBConnectionMySQL.h>
#include <DBConnectionOracle.h>

#define CFG_EXT ".cfg"

#define LOCAL_DB_NAME "wdb_disk"

// ---------------------------------------------------------------------------
class TSettings : public TObject {
private:
	// При добавлении настройки, обновлять TSettings, Equals, Assign, ToString;

	String FOptionsPass;

	String FConfigFileName;

	TColor FColorChanged;
	TColor FColorReadOnly;
	TColor FColorSelected;

	bool FUseLocal;

	bool FScaleTypeDyn;

	TDBConnectionMySQL * FLocalConnection;
	TDBConnectionMySQL * FServerMySQLConnection;
	TDBConnectionOracle * FServerOracleConnection;

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

	__property TColor ColorChanged = {
		read = FColorChanged, write = FColorChanged};
	__property TColor ColorReadOnly = {
		read = FColorReadOnly, write = FColorReadOnly};
	__property TColor ColorSelected = {
		read = FColorSelected, write = FColorSelected};

	__property bool UseLocal = {read = FUseLocal, write = FUseLocal};

	__property bool ScaleTypeDyn = {read = FScaleTypeDyn, write = FScaleTypeDyn
	};

	__property TDBConnectionMySQL * LocalConnection = {read = FLocalConnection};
	__property TDBConnectionMySQL * ServerMySQLConnection = {
		read = FServerMySQLConnection};
	__property TDBConnectionOracle * ServerOracleConnection = {
		read = FServerOracleConnection};
};

// ---------------------------------------------------------------------------
#endif
