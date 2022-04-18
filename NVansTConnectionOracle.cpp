// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsStr.h>

#include "NVansStrings.h"

#include "NVansTConnectionOracle.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TConnectionOracle::TConnectionOracle() : TConnectionServer() {
	Port = 1521;
	FService = "";
}

// ---------------------------------------------------------------------------
bool __fastcall TConnectionOracle::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TConnectionOracle * Connection = (TConnectionOracle*) Obj;

	if (Driver != Connection->Driver || Host != Connection->Host ||
		Port != Connection->Port || Service != Connection->Service ||
		User != Connection->User || Password != Connection->Password)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TConnectionOracle::Assign(TConnectionInfo * Source) {
	TConnectionOracle * Connection = (TConnectionOracle*) Source;

	Driver = Connection->Driver;
	Host = Connection->Host;
	Port = Connection->Port;
	Service = Connection->Service;
	User = Connection->User;
	Password = Connection->Password;
}

// ---------------------------------------------------------------------------
String __fastcall TConnectionOracle::ToString() {
	String S;

	S = "TConnectionOracle{";
	S += "Driver='" + Driver + "'";
	S += ", ";
	S += "Host='" + Host + "'";
	S += ", ";
	S += "Port='" + Port + "'";
	S += ", ";
	S += "Service='" + Service + "'";
	S += ", ";
	S += "User='" + User + "'";
	S += ", ";
	S += "Password='" + Password + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
String TConnectionOracle::GetConnectionString() {
	return Format(IDS_CONNECTION_ORACLE,
		ARRAYOFCONST((Driver, Host, Service, User, Password)));
}

// ---------------------------------------------------------------------------
