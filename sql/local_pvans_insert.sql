INSERT INTO pvans
	(invnum, carrying, iscales_carrying, idatetime_carrying, loadnorm, iscales_loadnorm, idatetime_loadnorm,
	tare_t, iscales_tare_t, idatetime_tare_t)
VALUES
	(:INVNUM, :CARRYING, :ISCALES_CARRYING, :IDATETIME_CARRYING, :LOADNORM, :ISCALES_LOADNORM, :IDATETIME_LOADNORM,
	:TARE_T, :ISCALES_TARE_T, :IDATETIME_TARE_T)