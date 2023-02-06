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

#include "NVansSettings.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TSettings::TSettings() {
	FOptionsPass = "";

	FConfigFileName = GetConfigFileName();

	FSQLToLog = false;

	FScaleType = stDisabled;

	FLocalConnection = new TDBConnectionMySQL();
	FServerOracleConnection = new TDBConnectionOracle();

	FLocalConnection->Database = LOCAL_DB_NAME;
}

// ---------------------------------------------------------------------------
__fastcall TSettings::~TSettings() {
	FServerOracleConnection->Free();
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

	if (Settings->SQLToLog != SQLToLog)
		return false;

	if (Settings->ScaleType != ScaleType)
		return false;

	if (!Settings->LocalConnection->Equals(LocalConnection))
		return false;
	if (!Settings->ServerOracleConnection->Equals(ServerOracleConnection))
		return false;

	if (Settings->WMEProgramPath != WMEProgramPath)
		return false;
	if (Settings->WMEProgramParams != WMEProgramParams)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TSettings::Assign(TSettings * Source) {
	FOptionsPass = Source->OptionsPass;

	FSQLToLog = Source->SQLToLog;

	FScaleType = Source->ScaleType;

	FLocalConnection->Assign(Source->LocalConnection);
	FServerOracleConnection->Assign(Source->ServerOracleConnection);

	FWMEProgramPath = Source->WMEProgramPath;
	FWMEProgramParams = Source->WMEProgramParams;
}

// ---------------------------------------------------------------------------
String __fastcall TSettings::ToString() {
	String S;

	S = "TSettings{";
	S += "OptionsPass=" + OptionsPass;
	S += ",";

	S += "SQLToLog=" + BoolToStr(SQLToLog);
	S += ",";

	S += "ScaleType=" + IntToStr(ScaleType);
	S += ",";

	S += "LocalConnection=" + LocalConnection->ToString();
	S += ",";
	S += "ServerOracleConnection=" + ServerOracleConnection->ToString();
	S += ",";

	S += "WMEProgramPath=" + WMEProgramPath;
	S += ",";
	S += "WMEProgramParams=" + WMEProgramParams;

	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
String TSettings::GetConfigFileName() {
	return ChangeFileExt(Application->ExeName, CFG_EXT);
}

// ---------------------------------------------------------------------------
String TSettings::Encrypt(String Text) {
	return ::Encrypt(Text, ENC_KEY);
}

// ---------------------------------------------------------------------------
String TSettings::Decrypt(String Text) {
	return ::Decrypt(Text, ENC_KEY);
}

// ---------------------------------------------------------------------------
String TSettings::GetCRC() {
	String S = ToString();
	S = CRC(S);
	return S;
}

// ---------------------------------------------------------------------------
void TSettings::CheckCRC(String EncryptedCRC) {
#ifndef DISABLE_CHECK_CRC
	::CheckCRC(GetCRC(), EncryptedCRC, ENC_KEY);
#endif
}

// ---------------------------------------------------------------------------
void TSettings::LoadSettings() {
	TIniFile * IniFile = new TIniFile(ConfigFileName);

	String Section;

	try {
		// -------------------------------------------------------------------
		Section = "Main";
		OptionsPass = Decrypt(IniFile->ReadString(Section, "OptionsPass",
			Encrypt(OptionsPass)));

		SQLToLog = IniFile->ReadBool(Section, "SQLToLog", SQLToLog);

		ScaleType = (TScaleType)IniFile->ReadInteger(Section, "ScaleType",
			ScaleType);

		// -------------------------------------------------------------------
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
			Encrypt(LocalConnection->Password)));
		LocalConnection->Driver = IniFile->ReadString(Section, "Driver",
			LocalConnection->Driver);

		// -------------------------------------------------------------------
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
			Encrypt(ServerOracleConnection->Password)));
		ServerOracleConnection->Driver =
			IniFile->ReadString(Section, "Driver",
			ServerOracleConnection->Driver);

		// -------------------------------------------------------------------
		Section = "WME";
		WMEProgramPath = IniFile->ReadString(Section, "ProgramPath",
			WMEProgramPath);
		WMEProgramParams = IniFile->ReadString(Section, "ProgramParams",
			WMEProgramParams);

		// -------------------------------------------------------------------
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
		IniFile->WriteString("CRC", "CRC", Encrypt(GetCRC()));

		// -------------------------------------------------------------------
		Section = "Main";
		IniFile->WriteString(Section, "Version", GetFileVer());

		IniFile->WriteString(Section, "OptionsPass", Encrypt(OptionsPass));

		IniFile->WriteBool(Section, "SQLToLog", SQLToLog);

		IniFile->WriteInteger(Section, "ScaleType", ScaleType);

		// -------------------------------------------------------------------
		Section = "LocalConnection";
		IniFile->WriteString(Section, "Host", LocalConnection->Host);
		IniFile->WriteString(Section, "Port", LocalConnection->Port);
		IniFile->WriteString(Section, "Database", LocalConnection->Database);
		IniFile->WriteString(Section, "User", LocalConnection->User);
		IniFile->WriteString(Section, "Pass",
			Encrypt(LocalConnection->Password));
		IniFile->WriteString(Section, "Driver", LocalConnection->Driver);

		// -------------------------------------------------------------------
		Section = "ServerOracleConnection";
		IniFile->WriteString(Section, "Host", ServerOracleConnection->Host);
		IniFile->WriteString(Section, "Port", ServerOracleConnection->Port);
		IniFile->WriteString(Section, "Service",
			ServerOracleConnection->Service);
		IniFile->WriteString(Section, "User", ServerOracleConnection->User);
		IniFile->WriteString(Section, "Pass",
			Encrypt(ServerOracleConnection->Password));
		IniFile->WriteString(Section, "Driver", ServerOracleConnection->Driver);

		// -------------------------------------------------------------------
		Section = "WME";
		IniFile->WriteString(Section, "ProgramPath", WMEProgramPath);
		IniFile->WriteString(Section, "ProgramParams", WMEProgramParams);
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
