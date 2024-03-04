// ---------------------------------------------------------------------------

#pragma hdrstop

#include <System.JSON.hpp>
#include <System.JSON.Writers.hpp>
#include <System.JSON.Builders.hpp>

#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsMisc.h>

#include "NVansAdd.h"

#include "NVansStrings.h"

#include "NVansSaveToFile.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

void SaveServerVanListToFileCsv(TOracleVanList * ServerVanList, String FileName)
{
	TStringList * Line = new TStringList();
	TStringList * List = new TStringList();

	try {
		Line->Delimiter = ';';
		Line->QuoteChar = '"';
		Line->StrictDelimiter = true;

		for (int i = 0; i < ServerVanList->Count; i++) {
			Line->Clear();

			// 0
			Line->Add(DateTimeToStr(ServerVanList->Items[i]->InvoiceDateTime));
			// 1
			Line->Add(ServerVanList->Items[i]->VanNum);
			// 2
			Line->Add(ServerVanList->Items[i]->CargoType);
			// 3
			Line->Add(IntToStr(ServerVanList->Items[i]->CargoTypeCode));
			// 4
			Line->Add(ServerVanList->Items[i]->InvoiceNum);
			// 5
			Line->Add(ServerVanList->Items[i]->InvoiceSupplier);
			// 6
			Line->Add(ServerVanList->Items[i]->InvoiceRecipient);
			// 7
			Line->Add(ServerVanList->Items[i]->DepartStation);
			// 8
			Line->Add(ServerVanList->Items[i]->PurposeStation);
			// 9
			Line->Add(IntToStr(ServerVanList->Items[i]->Carrying));
			// 10
			Line->Add(IntToStr(ServerVanList->Items[i]->TareT));
			// 11
			Line->Add(IntToStr(ServerVanList->Items[i]->InvoiceNetto));
			// 12
			Line->Add(IntToStr(ServerVanList->Items[i]->InvoiceTare));

			List->Add(Line->DelimitedText);
		}

		List->SaveToFile(FileName);
	}
	__finally {
		List->Free();
		Line->Free();
	}
}

// ---------------------------------------------------------------------------
String JsonStrReplace(String Value) {
	String Result = Trim(Value);

	for (int i = Result.Length(); i > 1; i--) {
		if (Result[i] == '\t' || Result[i] == '\r' || Result[i] == '\n' ||
			Result[i] == '"') {
			Result.Delete(i, 1);
		}
	}

	return Result;
}

// ---------------------------------------------------------------------------
String JsonTag(String Key, String Value) {
	if (Value.IsEmpty()) {
		return "\"" + Key + "\": null";
	}
	return "\"" + Key + "\": \"" + JsonStrReplace(Value) + "\"";
}

// ---------------------------------------------------------------------------
String JsonTag(String Key, TDateTime Value) {
	return JsonTag(Key, DateTimeToStr(Value));
}

// ---------------------------------------------------------------------------
String JsonTag(String Key, int Value) {
	return JsonTag(Key, IntToStr(Value));
}

// ---------------------------------------------------------------------------
void JsonListAddRecord(TStrings * JsonList, TOracleVan * Record) {
	JsonList->Add("{");

	JsonList->Add(JsonTag("InvoiceDateTime", Record->InvoiceDateTime) + ",");

	JsonList->Add(JsonTag("VanNum", Record->VanNum) + ",");

	JsonList->Add(JsonTag("CargoType", Record->CargoType) + ",");
	JsonList->Add(JsonTag("CargoTypeCode", Record->CargoTypeCode) + ",");

	JsonList->Add(JsonTag("InvoiceNum", Record->InvoiceNum) + ",");
	JsonList->Add(JsonTag("InvoiceSupplier", Record->InvoiceSupplier) + ",");
	JsonList->Add(JsonTag("InvoiceRecipient", Record->InvoiceRecipient) + ",");

	JsonList->Add(JsonTag("DepartStation", Record->DepartStation) + ",");
	JsonList->Add(JsonTag("PurposeStation", Record->PurposeStation) + ",");

	JsonList->Add(JsonTag("Carrying", Record->Carrying) + ",");
	JsonList->Add(JsonTag("TareT", Record->TareT) + ",");

	JsonList->Add(JsonTag("InvoiceNetto", Record->InvoiceNetto) + ",");
	JsonList->Add(JsonTag("InvoiceTare", Record->InvoiceTare) + ",");

	JsonList->Add("}");
}

// ---------------------------------------------------------------------------
void SaveServerVanListToFileJson(TOracleVanList * ServerVanList,
	String FileName) {
	TStringBuilder * StringBuilder = new TStringBuilder();
	TStringWriter * StringWriter = new TStringWriter(StringBuilder);
	TJsonTextWriter * JsonTextWriter = new TJsonTextWriter(StringWriter);
	TJSONObjectBuilder * Builder = new TJSONObjectBuilder(JsonTextWriter);
	TStringStream * StringStream = new TStringStream();

	JsonTextWriter->Formatting = TJsonFormatting::Indented;

	try {
		TJSONCollectionBuilder::TPairs * JsonObject = Builder->BeginObject();

		TJSONCollectionBuilder::TElements * Items =
			JsonObject->BeginArray("items");

		TJSONCollectionBuilder::TPairs * Item;

		for (int i = 0; i < ServerVanList->Count; i++) {
			Item = Items->BeginObject();

			Item->Add("InvoiceDateTime",
				ServerVanList->Items[i]->InvoiceDateTime);

			Item->Add("VanNum", ServerVanList->Items[i]->VanNum);

			Item->Add("CargoType", ServerVanList->Items[i]->CargoType);
			Item->Add("CargoTypeCode", ServerVanList->Items[i]->CargoTypeCode);

			Item->Add("InvoiceNum", ServerVanList->Items[i]->InvoiceNum);
			Item->Add("InvoiceSupplier",
				ServerVanList->Items[i]->InvoiceSupplier);
			Item->Add("InvoiceRecipient",
				ServerVanList->Items[i]->InvoiceRecipient);

			Item->Add("DepartStation", ServerVanList->Items[i]->DepartStation);
			Item->Add("PurposeStation",
				ServerVanList->Items[i]->PurposeStation);

			Item->Add("Carrying", ServerVanList->Items[i]->Carrying);
			Item->Add("TareT", ServerVanList->Items[i]->TareT);

			Item->Add("InvoiceNetto", ServerVanList->Items[i]->InvoiceNetto);
			Item->Add("InvoiceTare", ServerVanList->Items[i]->InvoiceTare);

			Item->EndObject();
		}

		Items->EndArray();

		JsonObject->EndObject();

		StringStream->WriteString(StringBuilder->ToString());
		StringStream->SaveToFile(FileName);
	}
	__finally {
		StringStream->Free();
		Builder->Free();
		JsonTextWriter->Free();
		StringWriter->Free();
		StringBuilder->Free();
	}
}

// ---------------------------------------------------------------------------
void SaveServerVanListToFile(TOracleVanList * ServerVanList, String FileName,
	TSaveFileType SaveFileType) {
	switch (SaveFileType) {
	case ftCsv:
		SaveServerVanListToFileCsv(ServerVanList, FileName);
		break;
	case ftJson:
		SaveServerVanListToFileJson(ServerVanList, FileName);
		break;
	default: ;
		throw Exception("unknown SaveServerVanListToFile SaveFileType fail");
	}
}

// ---------------------------------------------------------------------------
