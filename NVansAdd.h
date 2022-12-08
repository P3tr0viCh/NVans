// ---------------------------------------------------------------------------

#ifndef NVansAddH
#define NVansAddH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>

// ---------------------------------------------------------------------------
int DTToWTime(TDateTime ADateTime);
String DTToS(TDateTime ADateTime, bool WithSecs = true);

// ---------------------------------------------------------------------------
String CheckStrValue(String Value, int MaxLength);
bool CheckDateTimeValue(String Value);
bool CheckIntValue(String Value);

// ---------------------------------------------------------------------------
String NormalizeVanNumView(String Value);
String NormalizeVanNumSQL(String Value);

// ---------------------------------------------------------------------------
#endif
