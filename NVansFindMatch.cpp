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
	TFindMatchResult ExpectedFindMatchResult,
	int ExpectedFindMatchResultFirstIndex) {
	TIntegerList * Result = new TIntegerList();

	try {
		TFindMatchResult FindMatchResult = FindMatch(Source, Dest, Result);

		if (FindMatchResult != ExpectedFindMatchResult) {
			return true;
		}
		if (ExpectedFindMatchResultFirstIndex >= 0) {
			if (Result->Count != Source->Count) {
				return true;
			}
			if (Result->Count <= 0) {
				return true;
			}
			if (Result->Items[0]->Value != ExpectedFindMatchResultFirstIndex) {
				return true;
			}
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

	int FindMatchTestNum;

	try {
		// test empty
		FindMatchTestNum = 1;

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, -1)) {
			return FindMatchTestNum;
		}

		// test count
		FindMatchTestNum = 2;

		Source->Add("0001");
		Source->Add("0002");
		Source->Add("0003");

		Dest->Add("0003");

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, -1)) {
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

		if (FindMatchTestCheckFail(Source, Dest, fmFound, 1)) {
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

		if (FindMatchTestCheckFail(Source, Dest, fmFound, 1)) {
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

		if (FindMatchTestCheckFail(Source, Dest, fmFoundReverse, 3)) {
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

		if (FindMatchTestCheckFail(Source, Dest, fmFoundReverse, 3)) {
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

		if (FindMatchTestCheckFail(Source, Dest, fmFoundReverse, 4)) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, прямой порядок
		FindMatchTestNum = 8;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0001");
		Dest->Add("0002");

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, -1)) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, обратный порядок
		FindMatchTestNum = 9;

		Dest->Clear();
		Dest->Add("0002");
		Dest->Add("0001");
		Dest->Add("0000");

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, -1)) {
			return FindMatchTestNum;
		}

		// нет совпадений
		FindMatchTestNum = 10;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0000");
		Dest->Add("0000");

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, -1)) {
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

		if (FindMatchTestCheckFail(Source, Dest, fmFound, 1)) {
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

		if (FindMatchTestCheckFail(Source, Dest, fmFoundReverse, 4)) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, прямой порядок
		FindMatchTestNum = 13;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0002");
		Dest->Add("0003");

		if (FindMatchTestCheckFail(Source, Dest, fmNotFound, -1)) {
			return FindMatchTestNum;
		}
	}
	__finally {
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
bool FindMatchInt(TStringList * Source, TStringList * Dest, int &Index) {
	String VanNum = Source->Strings[0];

	bool Result = true;

	Index = -1;

	for (int d = 0; d < Dest->Count; d++) {
		if (SameStr(Dest->Strings[d], VanNum)) {
			Index = d;
			break;
		}
	}

	WriteToLogDebug("find first vannum result = " + IntToStr(Index));

	if (Index != -1) {
		if (Index + Source->Count > Dest->Count) {
			WriteToLogDebug("count fail");
			Index = -1;
			Result = false;
		}
		else {
			for (int s = 1, d = Index + 1; s < Source->Count && d < Dest->Count;
			s++, d++) {
				WriteToLogDebug("source = '" + Source->Strings[s] +
					"', dest = '" + Dest->Strings[d] + "'");

				if (IsEmpty(Dest->Strings[d])) {
					continue;
				}

				if (!SameStr(Dest->Strings[d], Source->Strings[s])) {
					WriteToLogDebug("vannums fail");
					Index = -1;
					Result = false;
					break;
				}
			}
		}
	}
	else {
		Result = false;
	}

	WriteToLogDebug("FindMatchInt result = " + BoolToStr(Result, true) +
		", index = " + IntToStr(Index));

	return Result;
}

// ---------------------------------------------------------------------------
TFindMatchResult FindMatch(TStringList * Source, TStringList * Dest,
	TIntegerList * Result) {
	WriteToLogDebug("start with first vannum");

	Result->Clear();

	if (Source->Count == 0) {
		WriteToLogDebug("FindMatch result = " + IntToStr(fmNotFound));
		return fmNotFound;
	}
	if (Dest->Count == 0) {
		WriteToLogDebug("FindMatch result = " + IntToStr(fmNotFound));
		return fmNotFound;
	}
	if (Source->Count > Dest->Count) {
		WriteToLogDebug("FindMatch result = " + IntToStr(fmNotFound));
		return fmNotFound;
	}

	bool Reverse = false;

	int Index = -1;

	if (!FindMatchInt(Source, Dest, Index)) {
		WriteToLogDebug("start with first vannum reverse");

		TStringList * DestReverse = new TStringList();

		for (int i = Dest->Count - 1; i >= 0; i--) {
			DestReverse->Add(Dest->Strings[i]);
		}

		Reverse = true;

		if (FindMatchInt(Source, DestReverse, Index)) {
			Index = Dest->Count - 1 - Index;
		}
		else {
			WriteToLogDebug("start with last vannum");

			TStringList * SourceReverse = new TStringList();

			for (int i = Source->Count - 1; i >= 0; i--) {
				SourceReverse->Add(Source->Strings[i]);
			}

			Reverse = false;

			if (FindMatchInt(SourceReverse, DestReverse, Index)) {
				Index = Dest->Count - Index - Source->Count;
			}
			else {
				WriteToLogDebug("start with last vannum reverse");

				Reverse = true;

				if (FindMatchInt(SourceReverse, Dest, Index)) {
					Index = Index + Source->Count - 1;
				}
			}

			SourceReverse->Free();
		}

		DestReverse->Free();
	}

	TFindMatchResult FindMatchResult;

	if (Index == -1) {
		FindMatchResult = fmNotFound;
	}
	else {
		if (Reverse) {
			FindMatchResult = fmFoundReverse;
			for (int i = 0; i < Source->Count; i++) {
				Result->Add(new TInteger(Index - i));
			}
		}
		else {
			FindMatchResult = fmFound;
			for (int i = 0; i < Source->Count; i++) {
				Result->Add(new TInteger(Index + i));
			}
		}
	}

	WriteToLogDebug("FindMatch result = " + IntToStr(FindMatchResult) +
		", indexes = " + Result->ToString());

	return FindMatchResult;
}

// ---------------------------------------------------------------------------
