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
bool StringGridIsEmpty(TStringGrid * Grid);
void StringGridClear(TStringGrid * Grid);
void StringGridSelectCell(TStringGrid * Grid, int ACol, int ARow);
void StringGridUpdateOrderNum(TStringGrid * Grid, int StartValue = 1);
void StringGridDeleteRow(TStringGrid * Grid, int ARow, int AColCount = -1);
void StringGridSetHeader(TStringGrid * Grid, int ACol, String ColName,
	int ColWidth);
void StringGridSetHeader(TStringGrid * Grid, int ACol, NativeUInt ColNameIdent,
	int ColWidth);
void StringGridDrawCell(TStringGrid * Grid, int ACol, int ARow, TRect Rect,
	TGridDrawState State, TIntegerSet ColsReadOnly, TIntegerSet ColsLeftAlign,
	TIntegerSet ColsCustomColor, TColor ReadOnlyColor, TColor CustomColor,
	bool DrawFocusedOnInactive, bool ReadOnlyRow = false);
void StringGridMouseToCell(TStringGrid * Grid, int &ACol, int &ARow);
void StringGridSelectRowAfterFixedCellClick(TStringGrid * Grid, int ARow);

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
