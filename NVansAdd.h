// ---------------------------------------------------------------------------

#ifndef NVansAddH
#define NVansAddH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>

// ---------------------------------------------------------------------------
const TIntegerSet NUSet;
const TColor NUColor = -1;

// ---------------------------------------------------------------------------
String LoadSQL(NativeUInt Ident);
String SQLMake(String S1, String S2);
String SQLMake(String S1, NativeUInt Ident);
TParameter * GetParam(TADOQuery * Query, String Name, TFieldType DataType);

// ---------------------------------------------------------------------------
String DateTimeToSQLStr(TDateTime ADateTime);
int DateTimeToWTime(TDateTime ADateTime);

// ---------------------------------------------------------------------------
String CheckStrValue(String Value, int MaxLength);
bool CheckDateTimeValue(String Value);
bool CheckIntValue(String Value);

// ---------------------------------------------------------------------------
#endif
