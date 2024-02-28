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

	FUseAutoReplace = false;

	FScaleType = stDisabled;

	FLocalConnection = new TDBConnectionMySQL();
	FOracleConnection = new TDBConnectionOracle();
	FIsvsConnection = new TDBConnectionMySQL();

	FLocalConnection->Database = LOCAL_DB_NAME;
	FIsvsConnection->Database = ISVS_DB_NAME;
}

// ---------------------------------------------------------------------------
__fastcall TSettings::~TSettings() {
	FIsvsConnection->Free();
	FOracleConnection->Free();
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

	if (Settings->UseAutoReplace != UseAutoReplace)
		return false;

	if (Settings->ScaleType != ScaleType)
		return false;

	if (!Settings->LocalConnection->Equals(LocalConnection))
		return false;
	if (!Settings->OracleConnection->Equals(OracleConnection))
		return false;
	if (!Settings->IsvsConnection->Equals(IsvsConnection))
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

	FUseAutoReplace = Source->UseAutoReplace;

	FScaleType = Source->ScaleType;

	FLocalConnection->Assign(Source->LocalConnection);
	FOracleConnection->Assign(Source->OracleConnection);
	FIsvsConnection->Assign(Source->IsvsConnection);

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

	S += "UseAutoReplace=" + BoolToStr(UseAutoReplace);
	S += ",";

	S += "ScaleType=" + IntToStr(ScaleType);
	S += ",";

	S += "LocalConnection=" + LocalConnection->ToString();
	S += ",";
	S += "OracleConnection=" + OracleConnection->ToString();
	S += ",";
	S += "IsvsConnection=" + IsvsConnection->ToString();
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

		UseAutoReplace = IniFile->ReadBool(Section, "UseAutoReplace",
			UseAutoReplace);

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
		OracleConnection->Host = IniFile->ReadString(Section, "Host",
			OracleConnection->Host);
		OracleConnection->Port = IniFile->ReadString(Section, "Port",
			OracleConnection->Port);
		OracleConnection->Service = IniFile->ReadString(Section, "Service",
			OracleConnection->Service);
		OracleConnection->User = IniFile->ReadString(Section, "User",
			OracleConnection->User);
		OracleConnection->Password =
			Decrypt(IniFile->ReadString(Section, "Pass",
			Encrypt(OracleConnection->Password)));
		OracleConnection->Driver = IniFile->ReadString(Section, "Driver",
			OracleConnection->Driver);

		// -------------------------------------------------------------------
		Section = "IsvsConnection";
		IsvsConnection->Host = IniFile->ReadString(Section, "Host",
			IsvsConnection->Host);
		IsvsConnection->Port = IniFile->ReadString(Section, "Port",
			IsvsConnection->Port);
		IsvsConnection->Database = IniFile->ReadString(Section, "Database",
			IsvsConnection->Database);
		IsvsConnection->User = IniFile->ReadString(Section, "User",
			IsvsConnection->User);
		IsvsConnection->Password =
			Decrypt(IniFile->ReadString(Section, "Pass",
			Encrypt(IsvsConnection->Password)));
		IsvsConnection->Driver = IniFile->ReadString(Section, "Driver",
			IsvsConnection->Driver);

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

		IniFile->WriteBool(Section, "UseAutoReplace", UseAutoReplace);

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
		IniFile->WriteString(Section, "Host", OracleConnection->Host);
		IniFile->WriteString(Section, "Port", OracleConnection->Port);
		IniFile->WriteString(Section, "Service", OracleConnection->Service);
		IniFile->WriteString(Section, "User", OracleConnection->User);
		IniFile->WriteString(Section, "Pass",
			Encrypt(OracleConnection->Password));
		IniFile->WriteString(Section, "Driver", OracleConnection->Driver);

		// -------------------------------------------------------------------
		Section = "IsvsConnection";
		IniFile->WriteString(Section, "Host", IsvsConnection->Host);
		IniFile->WriteString(Section, "Port", IsvsConnection->Port);
		IniFile->WriteString(Section, "Database", IsvsConnection->Database);
		IniFile->WriteString(Section, "User", IsvsConnection->User);
		IniFile->WriteString(Section, "Pass",
			Encrypt(IsvsConnection->Password));
		IniFile->WriteString(Section, "Driver", IsvsConnection->Driver);

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
