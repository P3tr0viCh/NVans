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
String IntegerPairListToStr(TIntegerPairList * IntegerPairList) {
	String S = "";
	for (int i = 0; i < IntegerPairList->Count; i++) {
		S = S + IntegerPairList->Items[i]->Int1->Value + ":" +
			IntegerPairList->Items[i]->Int2->Value + ",";
	}
	return S;
}

// ---------------------------------------------------------------------------
bool FindMatchTestCheckFail(TStringList * Source, TStringList * Dest,
	TIntegerPairList * ExpectedResult) {
	TIntegerPairList * Result = new TIntegerPairList();

	try {
		FindMatch(Source, Dest, Result);

		WriteToLog("Expected Result: " + IntegerPairListToStr(ExpectedResult));
		WriteToLog("........ Result: " + IntegerPairListToStr(Result));

		// return false;

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
void SetFindMatchTestNum(int Value, int &FindMatchTestNum) {
	FindMatchTestNum = Value;
	WriteToLog("-------------------------------");
	WriteToLog("Test #" + IntToStr(FindMatchTestNum));
}

// ---------------------------------------------------------------------------
int FindMatchTest() {
	TStringList * Source = new TStringList();
	TStringList * Dest = new TStringList();

	TIntegerPairList * Result = new TIntegerPairList();

	int FindMatchTestNum;

	try {
		// test empty
		SetFindMatchTestNum(1, FindMatchTestNum);

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// test 2
		SetFindMatchTestNum(2, FindMatchTestNum);

		Source->Add("0001");
		Source->Add("0002");
		Source->Add("0003");

		Dest->Add("0003");

		Result->Add(new TIntegerPair(2, 0));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// указан номер только первого вагона, прямой порядок
		SetFindMatchTestNum(3, FindMatchTestNum);

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0001");
		Dest->Add("");
		Dest->Add("");
		Dest->Add("0000");

		Result->Clear();
		Result->Add(new TIntegerPair(0, 1));
		Result->Add(new TIntegerPair(1, 2));
		Result->Add(new TIntegerPair(2, 3));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// указаны номера всех вагонов, прямой порядок
		SetFindMatchTestNum(4, FindMatchTestNum);

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0001");
		Dest->Add("0002");
		Dest->Add("0003");
		Dest->Add("0000");

		Result->Clear();
		Result->Add(new TIntegerPair(0, 1));
		Result->Add(new TIntegerPair(1, 2));
		Result->Add(new TIntegerPair(2, 3));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// указан номер только первого вагона, обратный порядок
		SetFindMatchTestNum(5, FindMatchTestNum);

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("");
		Dest->Add("");
		Dest->Add("0001");
		Dest->Add("0000");

		Result->Clear();
		Result->Add(new TIntegerPair(0, 3));
		Result->Add(new TIntegerPair(1, 2));
		Result->Add(new TIntegerPair(2, 1));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// указаны номера всех вагонов, обратный порядок
		SetFindMatchTestNum(6, FindMatchTestNum);

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0003");
		Dest->Add("0002");
		Dest->Add("0001");
		Dest->Add("0000");

		Result->Clear();
		Result->Add(new TIntegerPair(0, 3));
		Result->Add(new TIntegerPair(1, 2));
		Result->Add(new TIntegerPair(2, 1));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// указаны номера части вагонов, обратный порядок
		SetFindMatchTestNum(7, FindMatchTestNum);

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0000");
		Dest->Add("0003");
		Dest->Add("");
		Dest->Add("0001");
		Dest->Add("0000");

		Result->Clear();
		Result->Add(new TIntegerPair(0, 4));
		Result->Add(new TIntegerPair(1, 3));
		Result->Add(new TIntegerPair(2, 2));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, прямой порядок
		SetFindMatchTestNum(8, FindMatchTestNum);

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0001");
		Dest->Add("0002");

		Result->Clear();
		Result->Add(new TIntegerPair(0, 1));
		Result->Add(new TIntegerPair(1, 2));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, обратный порядок
		SetFindMatchTestNum(9, FindMatchTestNum);

		Dest->Clear();
		Dest->Add("0002");
		Dest->Add("0001");
		Dest->Add("0000");

		Result->Clear();
		Result->Add(new TIntegerPair(0, 1));
		Result->Add(new TIntegerPair(1, 0));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// нет совпадений
		SetFindMatchTestNum(10, FindMatchTestNum);

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0000");
		Dest->Add("0000");

		Result->Clear();

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// указан номер только последнего вагона, прямой порядок
		SetFindMatchTestNum(11, FindMatchTestNum);

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("");
		Dest->Add("");
		Dest->Add("0003");
		Dest->Add("0000");
		Dest->Add("0000");

		Result->Clear();
		Result->Add(new TIntegerPair(0, 1));
		Result->Add(new TIntegerPair(1, 2));
		Result->Add(new TIntegerPair(2, 3));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// указан номер только последнего вагона, обратный порядок
		SetFindMatchTestNum(12, FindMatchTestNum);

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0000");
		Dest->Add("0003");
		Dest->Add("");
		Dest->Add("");
		Dest->Add("0000");

		Result->Clear();
		Result->Add(new TIntegerPair(0, 4));
		Result->Add(new TIntegerPair(1, 3));
		Result->Add(new TIntegerPair(2, 2));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, прямой порядок
		SetFindMatchTestNum(13, FindMatchTestNum);

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0002");
		Dest->Add("0003");

		Result->Clear();
		Result->Add(new TIntegerPair(1, 1));
		Result->Add(new TIntegerPair(2, 2));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// поиск по номерам, вагоны идут не по порядку
		SetFindMatchTestNum(14, FindMatchTestNum);

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

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// поиск по номерам, вагоны идут не по порядку, дублируются
		SetFindMatchTestNum(15, FindMatchTestNum);

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
		Dest->Add("0002"); // 6
		Dest->Add("0000"); // 7

		Result->Clear();
		Result->Add(new TIntegerPair(2, 3));
		Result->Add(new TIntegerPair(3, 2));
		Result->Add(new TIntegerPair(4, 4));

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
			return FindMatchTestNum;
		}

		// неоднозначность
		SetFindMatchTestNum(16, FindMatchTestNum);

		Source->Clear();
		Source->Add("0001"); // 0
		Source->Add("0002"); // 1

		Dest->Clear();
		Dest->Add("0001"); // 0
		Dest->Add(""); // 1
		Dest->Add("0002"); // 2
		Dest->Add("0000"); // 7

		Result->Clear();

		if (FindMatchTestCheckFail(Source, Dest, Result)) {
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

// ---------------------------------------------------------------------------
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
int __fastcall CompareIntegerPair(void * Item1, void * Item2) {
	if (((TIntegerPair*)Item1)->Int1->Equals(((TIntegerPair*)Item2)->Int1)) {
		return ((TIntegerPair*)Item1)->Int2->Value - ((TIntegerPair*)Item2)
			->Int2->Value;
	}
	return ((TIntegerPair*)Item1)->Int1->Value - ((TIntegerPair*)Item2)
		->Int1->Value;
}

// ---------------------------------------------------------------------------
void FindMatch(TStringList * Source, TStringList * Dest,
	TIntegerPairList * Result) {

	TIntegerPairListList * Results = new TIntegerPairListList();

	TIntegerPairList * IntResult;

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

		// ----------- s++, d++ -----------
		IntResult = new TIntegerPairList();

		for (int d = FirstEntry, s = i; d < Dest->Count && s < Source->Count;
		d++, s++) {
			if (IsEmpty(Dest->Strings[d]) || AnsiSameStr(Dest->Strings[d],
				Source->Strings[s])) {
				IntResult->Add(new TIntegerPair(s, d));
			}
		}

		Results->Add(IntResult);

		// ----------- s++, d-- -----------
		IntResult = new TIntegerPairList();

		for (int d = FirstEntry, s = i; d >= 0 && s < Source->Count; d--, s++) {
			if (IsEmpty(Dest->Strings[d]) || AnsiSameStr(Dest->Strings[d],
				Source->Strings[s])) {
				IntResult->Add(new TIntegerPair(s, d));
			}
		}

		Results->Add(IntResult);

		// ----------- s--, d-- -----------
		IntResult = new TIntegerPairList();

		for (int d = FirstEntry, s = i; d >= 0 && s >= 0; d--, s--) {
			if (IsEmpty(Dest->Strings[d]) || AnsiSameStr(Dest->Strings[d],
				Source->Strings[s])) {
				IntResult->Add(new TIntegerPair(s, d));
			}
		}

		Results->Add(IntResult);

		// ----------- s--, d++ -----------
		IntResult = new TIntegerPairList();

		for (int d = FirstEntry, s = i; d < Dest->Count && s >= 0; d++, s--) {
			if (IsEmpty(Dest->Strings[d]) || AnsiSameStr(Dest->Strings[d],
				Source->Strings[s])) {
				IntResult->Add(new TIntegerPair(s, d));
			}
		}

		Results->Add(IntResult);
	}

	if (Results->Count == 0) {
		return;
	}

	Results->Sort(CompareIntegerPairListCount);

#ifdef FIND_MATCH_TEST
	WriteToLog("Results:");
	for (int i = 0; i < Results->Count; i++) {
		WriteToLog(IntegerPairListToStr(Results->Items[i]));
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
	WriteToLog("Results sorted:");
	for (int i = 0; i < Results->Count; i++) {
		WriteToLog(IntegerPairListToStr(Results->Items[i]));
	}
#endif

	for (int i = 0; i < Results->Count; i++) {
		for (int j = 0; j < Results->Items[i]->Count; j++) {
			Result->Add(new TIntegerPair(Results->Items[i]->Items[j]));
		}
	}

	Results->Free();

	Result->Sort(CompareIntegerPair);

#ifdef FIND_MATCH_TEST
	WriteToLog("Results united and sorted:");
	WriteToLog(IntegerPairListToStr(Result));
#endif

	for (int i = 0; i < Result->Count; i++) {
		for (int j = i + 1; j < Result->Count; j++) {
			if (Result->Items[i]->Int1->Equals(Result->Items[j]->Int1)) {
				Result->Clear();
				break;
			}
		}
	}
}

// ---------------------------------------------------------------------------
