// ---------------------------------------------------------------------------

#ifndef NVansSettingsH
#define NVansSettingsH

#include <System.Classes.hpp>

#include <DBConnectionMySQL.h>
#include <DBConnectionOracle.h>

#define CFG_EXT ".cfg"

#define LOCAL_DB_NAME "wdb_disk"

// ---------------------------------------------------------------------------
enum TScaleType {
	stDisabled = 0, stAvitekDyn, stAvitekSta, stWME
};

// ---------------------------------------------------------------------------
class TSettings : public TObject {
private:
	// При добавлении настройки, обновлять TSettings, Equals, Assign, ToString;

	String FOptionsPass;

	String FConfigFileName;

	bool FSQLToLog;

	TScaleType FScaleType;

	TDBConnectionMySQL * FLocalConnection;
	TDBConnectionOracle * FServerOracleConnection;

	String FWMEProgramPath;
	String FWMEProgramParams;

	// -----------------------------------------------------------------------
	String GetConfigFileName();

	String Encrypt(String Text);
	String Decrypt(String Text);

	String GetCRC();
	void CheckCRC(String EncryptedCRC);

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

	__property bool SQLToLog = {read = FSQLToLog, write = FSQLToLog};

	__property TScaleType ScaleType = {read = FScaleType, write = FScaleType};

	__property TDBConnectionMySQL * LocalConnection = {read = FLocalConnection};
	__property TDBConnectionOracle * ServerOracleConnection = {
		read = FServerOracleConnection};

	__property String WMEProgramPath = {
		read = FWMEProgramPath, write = FWMEProgramPath};
	__property String WMEProgramParams = {
		read = FWMEProgramParams, write = FWMEProgramParams};
};

// ---------------------------------------------------------------------------
#endif
