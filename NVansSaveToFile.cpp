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

		// 0
		Line->Add("InvoiceDateTime");
		// 1
		Line->Add("VanNum");
		// 2
		Line->Add("CargoType");
		// 3
		Line->Add("CargoTypeCode");
		// 4
		Line->Add("InvoiceNum");
		// 5
		Line->Add("InvoiceSupplier");
		// 6
		Line->Add("InvoiceRecipient");
		// 7
		Line->Add("DepartStation");
		// 8
		Line->Add("DepartStationCode");
		// 9
		Line->Add("PurposeStation");
		// 10
		Line->Add("PurposeStationCode");
		// 11
		Line->Add("Carrying");
		// 12
		Line->Add("TareT");
		// 13
		Line->Add("InvoiceNetto");
		// 14
		Line->Add("InvoiceTare");

		List->Add(Line->DelimitedText);

		for (int i = 0; i < ServerVanList->Count; i++) {
			Line->Clear();

			Line->Add(DateTimeToStr(ServerVanList->Items[i]->InvoiceDateTime));

			Line->Add(ServerVanList->Items[i]->VanNum);

			Line->Add(ServerVanList->Items[i]->CargoType);
			Line->Add(IntToStr(ServerVanList->Items[i]->CargoTypeCode));

			Line->Add(ServerVanList->Items[i]->InvoiceNum);

			Line->Add(ServerVanList->Items[i]->InvoiceSupplier);
			Line->Add(ServerVanList->Items[i]->InvoiceRecipient);

			Line->Add(ServerVanList->Items[i]->DepartStation);
			Line->Add(IntToStr(ServerVanList->Items[i]->DepartStationCode));
			Line->Add(ServerVanList->Items[i]->PurposeStation);
			Line->Add(IntToStr(ServerVanList->Items[i]->PurposeStationCode));

			Line->Add(IntToStr(ServerVanList->Items[i]->Carrying));
			Line->Add(IntToStr(ServerVanList->Items[i]->TareT));

			Line->Add(IntToStr(ServerVanList->Items[i]->InvoiceNetto));
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
			Item->Add("DepartStationCode",
				ServerVanList->Items[i]->DepartStationCode);
			Item->Add("PurposeStation",
				ServerVanList->Items[i]->PurposeStation);
			Item->Add("PurposeStationCode",
				ServerVanList->Items[i]->PurposeStationCode);

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
