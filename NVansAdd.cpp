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
int FindMatchTest() {
	TStringList * Source = new TStringList();
	TStringList * Dest = new TStringList();

	bool Reverse = true;

	int FindMatchResult;
	int FindMatchTestNum;

	try {
		// test empty
		FindMatchTestNum = 1;

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != FIND_MATCH_RESULT_NOT_FOUND) {
			return FindMatchTestNum;
		}

		// test count
		FindMatchTestNum = 2;

		Source->Add("0001");
		Source->Add("0002");
		Source->Add("0003");

		Dest->Add("0003");

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != FIND_MATCH_RESULT_NOT_FOUND) {
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

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != 1 || Reverse != false) {
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

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != 1 || Reverse != false) {
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

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != 3 || Reverse != true) {
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

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != 3 || Reverse != true) {
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

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != 4 || Reverse != true) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, прямой порядок
		FindMatchTestNum = 8;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0001");
		Dest->Add("0002");

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != FIND_MATCH_RESULT_NOT_FOUND) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, обратный порядок
		FindMatchTestNum = 9;

		Dest->Clear();
		Dest->Add("0002");
		Dest->Add("0001");
		Dest->Add("0000");

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != FIND_MATCH_RESULT_NOT_FOUND) {
			return FindMatchTestNum;
		}

		// нет совпадений
		FindMatchTestNum = 10;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0000");
		Dest->Add("0000");

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != FIND_MATCH_RESULT_NOT_FOUND) {
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

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != 1 || Reverse != false) {
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

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != 4 || Reverse != true) {
			return FindMatchTestNum;
		}

		// есть только часть вагонов, прямой порядок
		FindMatchTestNum = 13;

		Dest->Clear();
		Dest->Add("0000");
		Dest->Add("0002");
		Dest->Add("0003");

		FindMatchResult = FindMatch(Source, Dest, Reverse);
		if (FindMatchResult != FIND_MATCH_RESULT_NOT_FOUND) {
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
int FindMatchInt(TStringList * Source, TStringList * Dest) {
	String VanNum = Source->Strings[0];

	int Result = FIND_MATCH_RESULT_NOT_FOUND;

	for (int d = 0; d < Dest->Count; d++) {
		if (SameStr(Dest->Strings[d], VanNum)) {
			Result = d;
			break;
		}
	}

	WriteToLogDebug("find first vannum result = " + IntToStr(Result));

	if (Result != FIND_MATCH_RESULT_NOT_FOUND) {
		if (Result + Source->Count > Dest->Count) {
			WriteToLogDebug("count fail");
			Result = FIND_MATCH_RESULT_NOT_FOUND;
		}
		else {
			for (int s = 1, d = Result + 1;
			s < Source->Count && d < Dest->Count; s++, d++) {
				WriteToLogDebug("source = '" + Source->Strings[s] +
					"', dest = '" + Dest->Strings[d] + "'");

				if (IsEmpty(Dest->Strings[d])) {
					continue;
				}

				if (!SameStr(Dest->Strings[d], Source->Strings[s])) {
					WriteToLogDebug("vannums fail");
					Result = FIND_MATCH_RESULT_NOT_FOUND;
					break;
				}
			}
		}
	}

	WriteToLogDebug("FindMatchInt result = " + IntToStr(Result));

	return Result;
}

// ---------------------------------------------------------------------------
int FindMatch(TStringList * Source, TStringList * Dest, bool &Reverse) {
	WriteToLogDebug("start with first vannum");

	if (Source->Count == 0) {
		return FIND_MATCH_RESULT_NOT_FOUND;
	}
	if (Dest->Count == 0) {
		return FIND_MATCH_RESULT_NOT_FOUND;
	}
	if (Source->Count > Dest->Count) {
		return FIND_MATCH_RESULT_NOT_FOUND;
	}

	Reverse = false;

	int Result = FindMatchInt(Source, Dest);

	if (Result == FIND_MATCH_RESULT_NOT_FOUND) {
		WriteToLogDebug("start with first vannum reverse");

		TStringList * DestReverse = new TStringList();

		for (int i = Dest->Count - 1; i >= 0; i--) {
			DestReverse->Add(Dest->Strings[i]);
		}

		Reverse = true;

		Result = FindMatchInt(Source, DestReverse);

		if (Result != FIND_MATCH_RESULT_NOT_FOUND) {
			Result = Dest->Count - 1 - Result;
		}
		else {
			WriteToLogDebug("start with last vannum");

			TStringList * SourceReverse = new TStringList();

			for (int i = Source->Count - 1; i >= 0; i--) {
				SourceReverse->Add(Source->Strings[i]);
			}

			Reverse = false;

			Result = FindMatchInt(SourceReverse, DestReverse);

			if (Result != FIND_MATCH_RESULT_NOT_FOUND) {
				Result = Dest->Count - Result - Source->Count;
			}
			else {
				WriteToLogDebug("start with last vannum reverse");

				Reverse = true;

				Result = FindMatchInt(SourceReverse, Dest);

				if (Result != FIND_MATCH_RESULT_NOT_FOUND) {
					Result = Result + Source->Count - 1;
				}
			}

			SourceReverse->Free();
		}

		DestReverse->Free();
	}

	WriteToLogDebug("FindMatch result = " + IntToStr(Result));

	return Result;
}

// ---------------------------------------------------------------------------
