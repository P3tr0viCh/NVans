//---------------------------------------------------------------------------

#ifndef NVansFindMatchH
#define NVansFindMatchH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <Integer.h>

//---------------------------------------------------------------------------
enum TFindMatchResult {
		fmNotFound = 0, fmFound, fmFoundReverse
	};

// ---------------------------------------------------------------------------
int FindMatchTest();

//---------------------------------------------------------------------------
TFindMatchResult FindMatch(TStringList * Source, TStringList * Dest, TIntegerList * Result);

//---------------------------------------------------------------------------
#endif
