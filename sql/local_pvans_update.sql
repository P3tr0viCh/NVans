UPDATE pvans
SET carrying=:CARRYING, iscales_carrying=:ISCALES_CARRYING, idatetime_carrying=:IDATETIME_CARRYING,
	loadnorm=:LOADNORM, iscales_loadnorm=:ISCALES_LOADNORM, idatetime_loadnorm=:IDATETIME_LOADNORM,
	tare_t=:TARE_T, iscales_tare_t=:ISCALES_TARE_T, idatetime_tare_t=:IDATETIME_TARE_T
WHERE invnum=:INVNUM