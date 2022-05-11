// ---------------------------------------------------------------------------

#pragma hdrstop

#include <System.SysUtils.hpp>
#include <System.DateUtils.hpp>

#include <UtilsStr.h>

#include "NVansStrings.h"

#include "NVansAdd.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
String LoadSQL(NativeUInt Ident) {
	String Result;

	TStrings * Strings = new TStringList;

	TResourceStream * Stream = new TResourceStream((int)HInstance,
		LoadStr(Ident), RT_RCDATA);

	try {
		Strings->LoadFromStream(Stream);

		Result = Strings->Text;
	}
	__finally {
		Stream->Free();
		Strings->Free();
	}

	return Result;
}

// ---------------------------------------------------------------------------
String SQLMake(String S1, String S2) {
	return S1 + SPACE + S2;
}

// ---------------------------------------------------------------------------
String SQLMake(String S1, NativeUInt Ident) {
	return S1 + SPACE + LoadStr(Ident);
}

// ---------------------------------------------------------------------------
TParameter * GetParam(TADOQuery * Query, String Name, TFieldType DataType) {
	TParameter * Parameter = Query->Parameters->ParamByName(Name);
	Parameter->DataType = DataType;
	return Parameter;
}

// ---------------------------------------------------------------------------
TParameter * GetParam(TADOQuery * Query, String Name, TFieldType DataType,
	Variant Value) {
	TParameter * Parameter = GetParam(Query, Name, DataType);
	Parameter->Value = Value;
	return Parameter;
}

// ---------------------------------------------------------------------------
String DateTimeToSQLStr(TDateTime ADateTime) {
	return FormatDateTime("yyyyMMddhhnnss", ADateTime);
}

// ---------------------------------------------------------------------------
int DateTimeToWTime(TDateTime ADateTime) {
	// TODO: Magic Number
	return int(DateTimeToUnix(IncHour(ADateTime, -3)));
}

// ---------------------------------------------------------------------------
String CheckStrValue(String Value, int MaxLength) {
	if (Value.IsEmpty()) {
		return Value;
	}

	int P = Value.Pos(sLineBreak);

	if (P > 0) {
		Value = Value.SubString(0, P - 1);
	}

	Value = StringReplace(Value, "\t", " ", TReplaceFlags() << rfReplaceAll);

	Value = Value.Trim();

	if (Value.Length() > MaxLength) {
		Value.SetLength(MaxLength);
	}

	return Value;
}

// ---------------------------------------------------------------------------
bool CheckDateTimeValue(String Value) {
	try {
		StrToDateTime(Value);
	}
	catch (...) {
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
bool CheckIntValue(String Value) {
	try {
		StrToInt(Value);
	}
	catch (...) {
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
