// ---------------------------------------------------------------------------

#pragma hdrstop

#include <System.SysUtils.hpp>
#include <System.DateUtils.hpp>

#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsLog.h>

#include "NVansDebug.h"

#include "NVansStrings.h"

#include "NVansAdd.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
int DTToWTime(TDateTime ADateTime) {
	// TODO: Magic Number
	return int(DateTimeToUnix(IncHour(ADateTime, -3)));
}

// ---------------------------------------------------------------------------
String DTToS(TDateTime ADateTime, bool WithSecs) {
	return WithSecs ? FormatDateTime("dd.MM.yyyy hh:nn:ss", ADateTime) :
		FormatDateTime("dd.MM.yyyy hh:nn", ADateTime);
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
