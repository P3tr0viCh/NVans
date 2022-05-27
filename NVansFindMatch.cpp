// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsLog.h>

#include "NVansDebug.h"

#include "NVansFindMatch.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
bool FindMatchTestCheckFail(TStringList * Source, TStringList * Dest,
	TFindMatchResult ExpectedFindMatchResult, TIntegerPairList * ExpectedResult)
{
	TIntegerPairList * Result = new TIntegerPairList();

	try {
		TFindMatchResult FindMatchResult = FindMatch(Source, Dest, Result);

		return false; // TODO
		if (FindMatchResult != ExpectedFindMatchResult) {
			return true;
		}

		if (ExpectedFindMatchResult == fmNotFound) {
			return false;
		}

		if (!Result->Equals(ExpectedResult)) {
			return true;
		}
	}
	__finally {
		Result->Free();
	}

	return false;
}

// ---------------------------------------------------------------------------
int FindMatchTest() {
	TStringList * Source = new TStringList();
	TStringList * Dest = new TStringList();

	TIntegerPairList * Result = new TIntegerPairList();

	int FindMatchTestNum;

	try {
		// test empty
		FindMatchTestNum = 1;

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, Result)) {
			return FindMatchTestNum;
		}

		// test count
		FindMatchTestNum = 2;

		Source->Add("0001");
		Source->Add("0002");
		Source->Add("0003");

		Dest->Add("0003");

		Result->Add(new TIntegerPair(0, -1));
		Result->Add(new TIntegerPair(1, -1));
		Result->Add(new TIntegerPair(2, -1));

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, Result)) {
			return FindMatchTestNum;
		}

		// указан номер только первого вагона, прямой порядок
		FindMatchTestNum = 3;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0001");
		Dest->Add("");
		Dest->Add("");
		Dest->Add("0000");

		Result->Items[0]->Int2->Value = 1;
		Result->Items[1]->Int2->Value = 2;
		Result->Items[2]->Int2->Value = 3;

		if (FindMatchTestCheckFail(Source, Dest, fmFound, Result)) {
			return FindMatchTestNum;
		}

		// указаны номера всех вагонов, прямой порядок
		FindMatchTestNum = 4;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0001");
		Dest->Add("0002");
		Dest->Add("0003");
		Dest->Add("0000");

		Result->Items[0]->Int2->Value = 1;
		Result->Items[1]->Int2->Value = 2;
		Result->Items[2]->Int2->Value = 3;

		if (FindMatchTestCheckFail(Source, Dest, fmFound, Result)) {
			return FindMatchTestNum;
		}

		// указан номер только первого вагона, обратный порядок
		FindMatchTestNum = 5;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("");
		Dest->Add("");
		Dest->Add("0001");
		Dest->Add("0000");

		Result->Items[0]->Int2->Value = 3;
		Result->Items[1]->Int2->Value = 2;
		Result->Items[2]->Int2->Value = 1;

		if (FindMatchTestCheckFail(Source, Dest, fmFoundReverse, Result)) {
			return FindMatchTestNum;
		}

		// указаны номера всех вагонов, обратный порядок
		FindMatchTestNum = 6;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0003");
		Dest->Add("0002");
		Dest->Add("0001");
		Dest->Add("0000");

		Result->Items[0]->Int2->Value = 3;
		Result->Items[1]->Int2->Value = 2;
		Result->Items[2]->Int2->Value = 1;

		if (FindMatchTestCheckFail(Source, Dest, fmFoundReverse, Result)) {
			return FindMatchTestNum;
		}

		// указаны номера части вагонов, обратный порядок
		FindMatchTestNum = 7;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0000");
		Dest->Add("0003");
		Dest->Add("");
		Dest->Add("0001");
		Dest->Add("0000");

		Result->Items[0]->Int2->Value = 4;
		Result->Items[1]->Int2->Value = 3;
		Result->Items[2]->Int2->Value = 2;

		if (FindMatchTestCheckFail(Source, Dest, fmFoundReverse, Result)) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, прямой порядок
		FindMatchTestNum = 8;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0001");
		Dest->Add("0002");

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, Result)) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, обратный порядок
		FindMatchTestNum = 9;

		Dest->Clear();
		Dest->Add("0002");
		Dest->Add("0001");
		Dest->Add("0000");

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, Result)) {
			return FindMatchTestNum;
		}

		// нет совпадений
		FindMatchTestNum = 10;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0000");
		Dest->Add("0000");

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, Result)) {
			return FindMatchTestNum;
		}

		// указан номер только последнего вагона, прямой порядок
		FindMatchTestNum = 11;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("");
		Dest->Add("");
		Dest->Add("0003");
		Dest->Add("0000");
		Dest->Add("0000");

		Result->Items[0]->Int2->Value = 1;
		Result->Items[1]->Int2->Value = 2;
		Result->Items[2]->Int2->Value = 3;

		if (FindMatchTestCheckFail(Source, Dest, fmFound, Result)) {
			return FindMatchTestNum;
		}

		// указан номер только последнего вагона, обратный порядок
		FindMatchTestNum = 12;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0000");
		Dest->Add("0003");
		Dest->Add("");
		Dest->Add("");
		Dest->Add("0000");

		Result->Items[0]->Int2->Value = 4;
		Result->Items[1]->Int2->Value = 3;
		Result->Items[2]->Int2->Value = 2;

		if (FindMatchTestCheckFail(Source, Dest, fmFoundReverse, Result)) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, прямой порядок
		FindMatchTestNum = 13;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0002");
		Dest->Add("0003");

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, Result)) {
			return FindMatchTestNum;
		}

		// поиск по номерам, вагоны идут не по порядку
		FindMatchTestNum = 14;

		Source->Clear();
		Source->Add("0010"); // 0
		Source->Add("0011"); // 1
		Source->Add("0001"); // 2
		Source->Add("0002"); // 3
		Source->Add("0003"); // 4
		Source->Add("0004"); // 5
		Source->Add("0005"); // 6

		Dest->Clear();
		Dest->Add("0000"); // 0
		Dest->Add("0000"); // 1
		Dest->Add("0002"); // 2
		Dest->Add("0001"); // 3
		Dest->Add("0003"); // 4
		Dest->Add("0000"); // 5

		Result->Clear();
		Result->Add(new TIntegerPair(2, 3));
		Result->Add(new TIntegerPair(3, 2));
		Result->Add(new TIntegerPair(4, 4));

		if (FindMatchTestCheckFail(Source, Dest, fmFoundVanNums, Result)) {
			return FindMatchTestNum;
		}
	}
	__finally {
		Result->Free();
		Dest->Free();
		Source->Free();
	}

	return 0;
}

// ---------------------------------------------------------------------------
void WriteToLogDebug(String S) {
#ifdef FIND_MATCH_TEST
	WriteToLog(S);
#endif
}

// ---------------------------------------------------------------------------
// bool FindMatchInt(TStringList * Source, TStringList * Dest, int &Index) {
// String VanNum = Source->Strings[0];
//
// bool Result = true;
//
// Index = -1;
//
// for (int d = 0; d < Dest->Count; d++) {
// if (AnsiSameStr(Dest->Strings[d], VanNum)) {
// Index = d;
// break;
// }
// }
//
// WriteToLogDebug("find first vannum result = " + IntToStr(Index));
//
// if (Index != -1) {
// if (Index + Source->Count > Dest->Count) {
// WriteToLogDebug("count fail");
// Index = -1;
// Result = false;
// }
// else {
// for (int s = 1, d = Index + 1; s < Source->Count && d < Dest->Count;
// s++, d++) {
// WriteToLogDebug("source = '" + Source->Strings[s] +
// "', dest = '" + Dest->Strings[d] + "'");
//
// if (IsEmpty(Dest->Strings[d])) {
// continue;
// }
//
// if (!AnsiSameStr(Dest->Strings[d], Source->Strings[s])) {
// WriteToLogDebug("vannums fail");
// Index = -1;
// Result = false;
// break;
// }
// }
// }
// }
// else {
// Result = false;
// }
//
// WriteToLogDebug("FindMatchInt result = " + BoolToStr(Result, true) +
// ", index = " + IntToStr(Index));
//
// return Result;
// }

// ---------------------------------------------------------------------------
int FindMatchFirstEntry(String Source, TStringList * Dest) {
	for (int i = 0; i < Dest->Count; i++) {
		if (AnsiSameStr(Dest->Strings[i], Source)) {
			return i;
		}
	}

	return -1;
}

// ---------------------------------------------------------------------------
typedef TObjList<TIntegerPairList>TIntegerPairListList;

// ---------------------------------------------------------------------------
int __fastcall CompareIntegerPairListCount(void * Item1, void * Item2) {
	return ((TIntegerPairList*)Item2)->Count - ((TIntegerPairList*)Item1)
		->Count;
}

bool CompareIntegerPairList(TIntegerPairList * Item1, TIntegerPairList * Item2)
{
	for (int i = 0; i < Item1->Count; i++) {
		for (int j = 0; j < Item2->Count; j++) {
			if (Item1->Items[i]->Equals(Item2->Items[j])) {
				return true;
			}
		}
	}
	return false;
}

// ---------------------------------------------------------------------------
TFindMatchResult FindMatch(TStringList * Source, TStringList * Dest,
	TIntegerPairList * Result) {

	TFindMatchResult FindMatchResult;

	TIntegerPairListList * Results = new TIntegerPairListList();

	TIntegerPairList * IntResult;

	WriteToLogDebug("Start FindMatch");

#ifdef FIND_MATCH_TEST
	String S;
	WriteToLog("Source:");
	for (int i = 0; i < Source->Count; i++) {
		S = S + Source->Strings[i] + ",";
	}
	WriteToLog(S);
	WriteToLog("Dest:");
	S = "";
	for (int i = 0; i < Dest->Count; i++) {
		S = S + Dest->Strings[i] + ",";
	}
	WriteToLog(S);
#endif

	int FirstEntry;

	for (int i = 0; i < Source->Count; i++) {
		FirstEntry = FindMatchFirstEntry(Source->Strings[i], Dest);

		if (FirstEntry == -1) {
			continue;
		}

		IntResult = new TIntegerPairList();

		for (int d = FirstEntry, s = i; d < Dest->Count && s < Source->Count;
		d++, s++) {
			if (IsEmpty(Dest->Strings[d]) || AnsiSameStr(Dest->Strings[d],
				Source->Strings[s])) {
				IntResult->Add(new TIntegerPair(s, d));
			}
		}

		Results->Add(IntResult);

		IntResult = new TIntegerPairList();

		for (int d = FirstEntry, s = i; d >= 0 && s < Source->Count; d--, s++) {
			if (IsEmpty(Dest->Strings[d]) || AnsiSameStr(Dest->Strings[d],
				Source->Strings[s])) {
				IntResult->Add(new TIntegerPair(s, d));
			}
		}

		Results->Add(IntResult);

        // TODO: d--, s-- (test 11); d++, s-- (test 12)
	}

	if (Results->Count == 0) {
		WriteToLogDebug("FindMatch result = " + IntToStr(fmNotFound));
		return fmNotFound;
	}

	Results->Sort(CompareIntegerPairListCount);

#ifdef FIND_MATCH_TEST
	WriteToLogDebug("Results:");
	for (int i = 0; i < Results->Count; i++) {
		S = "";
		for (int j = 0; j < Results->Items[i]->Count; j++) {
			S = S + Results->Items[i]->Items[j]->Int1->Value + ":" +
				Results->Items[i]->Items[j]->Int2->Value + ",";
		}
		WriteToLogDebug(S);
	}
#endif

	for (int i = Results->Count - 1; i > 0; i--) {
		for (int j = i - 1; j >= 0; j--) {
			if (CompareIntegerPairList(Results->Items[i], Results->Items[j])) {
				Results->Remove(Results->Items[i]);
				break;
			}
		}
	}

#ifdef FIND_MATCH_TEST
	WriteToLogDebug("Results sorted:");
	for (int i = 0; i < Results->Count; i++) {
		S = "";
		for (int j = 0; j < Results->Items[i]->Count; j++) {
			S = S + Results->Items[i]->Items[j]->Int1->Value + ":" +
				Results->Items[i]->Items[j]->Int2->Value + ",";
		}
		WriteToLogDebug(S);
	}
#endif

	Result->Assign(Results->Items[0]); // TODO: count > 1

	Results->Free();

#ifdef FIND_MATCH_TEST
	S = "";
	for (int i = 0; i < Result->Count; i++) {
		S = S + Result->Items[i]->Int1->Value + ":" +
			Result->Items[i]->Int2->Value + ",";
	}
	WriteToLogDebug("FindMatch result = " + IntToStr(FindMatchResult) +
		", pairs = " + S);
#endif

	return FindMatchResult;

	// WriteToLogDebug("start with first vannum");
	//
	// Result->Clear();
	//
	// if (Source->Count == 0) {
	// WriteToLogDebug("FindMatch result = " + IntToStr(fmNotFound));
	// return fmNotFound;
	// }
	// if (Dest->Count == 0) {
	// WriteToLogDebug("FindMatch result = " + IntToStr(fmNotFound));
	// return fmNotFound;
	// }
	//
	// if (Source->Count > Dest->Count) {
	// WriteToLogDebug("FindMatch result = " + IntToStr(fmNotFound));
	// return fmNotFound;
	// }
	//
	// bool Reverse = false;
	//
	// int Index = -1;
	//
	// if (!FindMatchInt(Source, Dest, Index)) {
	// WriteToLogDebug("start with first vannum reverse");
	//
	// TStringList * DestReverse = new TStringList();
	//
	// for (int i = Dest->Count - 1; i >= 0; i--) {
	// DestReverse->Add(Dest->Strings[i]);
	// }
	//
	// Reverse = true;
	//
	// if (FindMatchInt(Source, DestReverse, Index)) {
	// Index = Dest->Count - 1 - Index;
	// }
	// else {
	// WriteToLogDebug("start with last vannum");
	//
	// TStringList * SourceReverse = new TStringList();
	//
	// for (int i = Source->Count - 1; i >= 0; i--) {
	// SourceReverse->Add(Source->Strings[i]);
	// }
	//
	// Reverse = false;
	//
	// if (FindMatchInt(SourceReverse, DestReverse, Index)) {
	// Index = Dest->Count - Index - Source->Count;
	// }
	// else {
	// WriteToLogDebug("start with last vannum reverse");
	//
	// Reverse = true;
	//
	// if (FindMatchInt(SourceReverse, Dest, Index)) {
	// Index = Index + Source->Count - 1;
	// }
	// }
	//
	// SourceReverse->Free();
	// }
	//
	// DestReverse->Free();
	// }
	//
	// TFindMatchResult FindMatchResult;
	//
	// if (Index == -1) {
	// FindMatchResult = fmNotFound;
	// }
	// else {
	// if (Reverse) {
	// FindMatchResult = fmFoundReverse;
	// for (int i = 0; i < Source->Count; i++) {
	// Result->Add(new TIntegerPair(i, Index - i));
	// }
	// }
	// else {
	// FindMatchResult = fmFound;
	// for (int i = 0; i < Source->Count; i++) {
	// Result->Add(new TIntegerPair(i, Index + i));
	// }
	// }
	// }
}

// ---------------------------------------------------------------------------
