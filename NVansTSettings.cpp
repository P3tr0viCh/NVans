// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsFiles.h>
#include <UtilsFileIni.h>
#include <UtilsCryptoPP.h>

#include "NVansEncKey.h"

#include "NVansDebug.h"

#include "NVansStrings.h"

#include "NVansTSettings.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TSettings::TSettings() {
	FOptionsPass = "";

	FConfigFileName = GetConfigFileName();

	FColorChanged = TColor(0x000085FB); // TODO: save
	FColorReadOnly = TColor(0x00E8E8E8);
	FColorSelected = clHotLight;

	FUseLocal = false;

	FScaleTypeDyn = true;

	FLocalConnection = new TConnectionMySQL();
	FServerMySQLConnection = new TConnectionMySQL();
	FServerOracleConnection = new TConnectionOracle();

	FLocalConnection->Database = LOCAL_DB_NAME;
}

// ---------------------------------------------------------------------------
__fastcall TSettings::~TSettings() {
	FServerOracleConnection->Free();
	FServerMySQLConnection->Free();
	FLocalConnection->Free();
}

// ---------------------------------------------------------------------------
bool __fastcall TSettings::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TSettings * Settings = (TSettings*) Obj;

	if (Settings->OptionsPass != OptionsPass)
		return false;

	if (Settings->ColorReadOnly != ColorReadOnly)
		return false;

	if (Settings->UseLocal != UseLocal)
		return false;

	if (Settings->ScaleTypeDyn != ScaleTypeDyn)
		return false;

	if (!Settings->LocalConnection->Equals(LocalConnection))
		return false;
	if (!Settings->ServerMySQLConnection->Equals(ServerMySQLConnection))
		return false;
	if (!Settings->ServerOracleConnection->Equals(ServerOracleConnection))
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TSettings::Assign(TSettings * Source) {
	FOptionsPass = Source->OptionsPass;

	FColorReadOnly = Source->ColorReadOnly;

	FUseLocal = Source->UseLocal;

	FScaleTypeDyn = Source->ScaleTypeDyn;

	FLocalConnection->Assign(Source->LocalConnection);
	FServerMySQLConnection->Assign(Source->ServerMySQLConnection);
	FServerOracleConnection->Assign(Source->ServerOracleConnection);
}

// ---------------------------------------------------------------------------
String __fastcall TSettings::ToString() {
	String S;

	S = "TSettings{";
	S += "OptionsPass='" + OptionsPass + "'";
	S += ",";
	S += "ColorReadOnly='" + ColorToString(ColorReadOnly) + "'";
	S += ",";
	S += "UseLocal='" + BoolToStr(UseLocal) + "'";
	S += ",";
	S += "ScaleTypeDyn='" + BoolToStr(ScaleTypeDyn) + "'";
	S += ",";
	S += "LocalConnection=" + LocalConnection->ToString();
	S += ",";
	S += "ServerMySQLConnection=" + ServerMySQLConnection->ToString();
	S += ",";
	S += "ServerOracleConnection=" + ServerOracleConnection->ToString();

	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
String TSettings::GetConfigFileName() {
	return ChangeFileExt(Application->ExeName, CFG_EXT);
}

// ---------------------------------------------------------------------------
String TSettings::CRC(String Text) {
	try {
		return HashSHA256(Text);
	}
	catch (...) {
		return "";
	}
}

// ---------------------------------------------------------------------------
String TSettings::Encrypt(String Text) {
	if (Text.IsEmpty()) {
		return "";
	}

	try {
		return EncryptAES(Text, ENC_KEY);
	}
	catch (...) {
		return "";
	}
}

// ---------------------------------------------------------------------------
String TSettings::Decrypt(String Text) {
	if (Text.IsEmpty()) {
		return "";
	}

	try {
		return DecryptAES(Text, ENC_KEY);
	}
	catch (...) {
		throw EEncodingError("decrypt");
	}
}

// ---------------------------------------------------------------------------
String TSettings::GetCRC() {
	String S = ToString();
	S = CRC(S);
	S = Encrypt(S);
	return S;
}

// ---------------------------------------------------------------------------
void TSettings::CheckCRC(String S) {
#ifdef DISABLE_CHECK_CRC
	return;
#endif

	if (S.IsEmpty()) {
		throw EEncodingError(IDS_LOG_ERROR_CRC_EMPTY);
	}

	S = Decrypt(S);

	String RightCRC = ToString();
	RightCRC = CRC(RightCRC);

	if (!SameStr(S, RightCRC)) {
		throw EEncodingError(IDS_LOG_ERROR_CRC_WRONG);
	}
}

// ---------------------------------------------------------------------------
void TSettings::LoadSettings() {
	TIniFile * IniFile = new TIniFile(ConfigFileName);

	String Section;

	try {
		Section = "Main";
		OptionsPass = Decrypt(IniFile->ReadString(Section, "OptionsPass",
			OptionsPass));
		ColorReadOnly = TColor(IniFile->ReadInteger(Section, "ColorReadOnly",
			ColorReadOnly));
		UseLocal = IniFile->ReadBool(Section, "UseLocal", UseLocal);
		ScaleTypeDyn = IniFile->ReadBool(Section, "ScaleTypeDyn", ScaleTypeDyn);

		Section = "LocalConnection";
		LocalConnection->Host = IniFile->ReadString(Section, "Host",
			LocalConnection->Host);
		LocalConnection->Port = IniFile->ReadString(Section, "Port",
			LocalConnection->Port);
		LocalConnection->Database = IniFile->ReadString(Section, "Database",
			LocalConnection->Database);
		LocalConnection->User = IniFile->ReadString(Section, "User",
			LocalConnection->User);
		LocalConnection->Password =
			Decrypt(IniFile->ReadString(Section, "Pass",
			LocalConnection->Password));
		LocalConnection->Driver = IniFile->ReadString(Section, "Driver",
			LocalConnection->Driver);

		Section = "ServerOracleConnection";
		ServerOracleConnection->Host =
			IniFile->ReadString(Section, "Host", ServerOracleConnection->Host);
		ServerOracleConnection->Port =
			IniFile->ReadString(Section, "Port", ServerOracleConnection->Port);
		ServerOracleConnection->Service =
			IniFile->ReadString(Section, "Service",
			ServerOracleConnection->Service);
		ServerOracleConnection->User =
			IniFile->ReadString(Section, "User", ServerOracleConnection->User);
		ServerOracleConnection->Password =
			Decrypt(IniFile->ReadString(Section, "Pass",
			ServerOracleConnection->Password));
		ServerOracleConnection->Driver =
			IniFile->ReadString(Section, "Driver",
			ServerOracleConnection->Driver);

		CheckCRC(IniFile->ReadString("CRC", "CRC", ""));
	}
	__finally {
		delete IniFile;
	}
}

// ---------------------------------------------------------------------------
void TSettings::SaveSettings() {
	TIniFile * IniFile = new TIniFile(ConfigFileName);

	String Section;

	try {
		IniFile->WriteString("CRC", "CRC", GetCRC());

		Section = "Main";
		IniFile->WriteString(Section, "Version", GetFileVer());
		IniFile->WriteString(Section, "OptionsPass", Encrypt(OptionsPass));
		IniFile->WriteInteger(Section, "ColorReadOnly", ColorReadOnly);
		IniFile->WriteBool(Section, "UseLocal", UseLocal);
		IniFile->WriteBool(Section, "ScaleTypeDyn", ScaleTypeDyn);

		Section = "LocalConnection";
		IniFile->WriteString(Section, "Host", LocalConnection->Host);
		IniFile->WriteString(Section, "Port", LocalConnection->Port);
		IniFile->WriteString(Section, "Database", LocalConnection->Database);
		IniFile->WriteString(Section, "User", LocalConnection->User);
		IniFile->WriteString(Section, "Pass",
			Encrypt(LocalConnection->Password));
		IniFile->WriteString(Section, "Driver", LocalConnection->Driver);

		Section = "ServerOracleConnection";
		IniFile->WriteString(Section, "Host", ServerOracleConnection->Host);
		IniFile->WriteString(Section, "Port", ServerOracleConnection->Port);
		IniFile->WriteString(Section, "Service",
			ServerOracleConnection->Service);
		IniFile->WriteString(Section, "User", ServerOracleConnection->User);
		IniFile->WriteString(Section, "Pass",
			Encrypt(ServerOracleConnection->Password));
		IniFile->WriteString(Section, "Driver", ServerOracleConnection->Driver);
	}
	__finally {
		delete IniFile;
	}
}

// ---------------------------------------------------------------------------
bool TSettings::Load() {
	if (!FileExists(ConfigFileName)) {
		WriteToLog(Format(IDS_LOG_ERROR_LOAD_SETTINGS, "not exists"));

		return true;
	}

	try {
		LoadSettings();
	}
	catch (Exception &E) {
		WriteToLog(Format(IDS_LOG_ERROR_LOAD_SETTINGS, E.Message));

		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
bool TSettings::Save() {
	try {
		SaveSettings();
	}
	catch (Exception &E) {
		WriteToLog(Format(IDS_LOG_ERROR_SAVE_SETTINGS, E.Message));

		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
