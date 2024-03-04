// ---------------------------------------------------------------------------

#ifndef NVansSaveToFileH
#define NVansSaveToFileH

#include "NVansOracleVan.h"

enum TSaveFileType {
	ftCsv, ftJson
};

// ---------------------------------------------------------------------------
void SaveServerVanListToFile(TOracleVanList * ServerVanList, String FileName,
	TSaveFileType SaveFileType);

// ---------------------------------------------------------------------------
#endif
