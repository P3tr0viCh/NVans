// ---------------------------------------------------------------------------

#ifndef NVansAddH
#define NVansAddH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <Vcl.Grids.hpp>

// ---------------------------------------------------------------------------
const TIntegerSet NUSet;
const TColor NUColor = -1;

// ---------------------------------------------------------------------------
String LoadSQL(NativeUInt Ident);

// ---------------------------------------------------------------------------
String DateTimeToSQLStr(TDateTime ADateTime);
int DateTimeToWTime(TDateTime ADateTime);

// ---------------------------------------------------------------------------
String CheckStrValue(String Value, int MaxLength);
bool CheckDateTimeValue(String Value);
bool CheckIntValue(String Value);

// ---------------------------------------------------------------------------
#endif
