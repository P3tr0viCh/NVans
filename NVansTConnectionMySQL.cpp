// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsStr.h>

#include "NVansStrings.h"

#include "NVansTConnectionMySQL.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TConnectionMySQL::TConnectionMySQL() : TConnectionServer() {
	Port = 3306;
	FDatabase = "";
}

// ---------------------------------------------------------------------------
bool __fastcall TConnectionMySQL::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TConnectionMySQL * Connection = (TConnectionMySQL*) Obj;

	if (Driver != Connection->Driver || Host != Connection->Host ||
		Port != Connection->Port || Database != Connection->Database ||
		User != Connection->User || Password != Connection->Password)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TConnectionMySQL::Assign(TConnection * Source) {
	TConnectionMySQL * Connection = (TConnectionMySQL*) Source;

	Driver = Connection->Driver;
	Host = Connection->Host;
	Port = Connection->Port;
	Database = Connection->Database;
	User = Connection->User;
	Password = Connection->Password;
}

// ---------------------------------------------------------------------------
String __fastcall TConnectionMySQL::ToString() {
	String S;

	S = "TConnectionMySQL{";
	S += "Driver='" + Driver + "'";
	S += ", ";
	S += "Host='" + Host + "'";
	S += ", ";
	S += "Port='" + Port + "'";
	S += ", ";
	S += "Database='" + Database + "'";
	S += ", ";
	S += "User='" + User + "'";
	S += ", ";
	S += "Password='" + Password + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
String TConnectionMySQL::GetConnectionString() {
	return Format(IDS_CONNECTION_MYSQL,
		ARRAYOFCONST((Driver, Host, Database, User, Password)));
}

// ---------------------------------------------------------------------------
