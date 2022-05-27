// ---------------------------------------------------------------------------

#ifndef NVansFindMatchH
#define NVansFindMatchH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include "IntegerPair.h"

// ---------------------------------------------------------------------------
enum TFindMatchResult {
	fmNotFound = 0, fmFound, fmFoundReverse, fmFoundVanNumsOrdered,
	fmFoundVanNums
};

// ---------------------------------------------------------------------------
int FindMatchTest();

// ---------------------------------------------------------------------------
TFindMatchResult FindMatch(TStringList * Source, TStringList * Dest,
	TIntegerPairList * Result);

// ---------------------------------------------------------------------------
#endif
