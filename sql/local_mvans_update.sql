UPDATE mvans
SET invnum=:INVNUM, carrying=:CARRYING, loadnorm=:LOADNORM, tare=:TARE, tare_t=:TARE_T, iscales_tare=:ISCALES_TARE, idatetime_tare=:IDATETIME_TARE,
	netto=:NETTO, overload=:OVERLOAD,
	cargotype=:CARGOTYPE, cargotype_code=:CARGOTYPE_CODE,
	depart_station=:DEPART_STATION, depart_station_code=:DEPART_STATION_CODE,
	purpose_station=:PURPOSE_STATION, purpose_station_code=:PURPOSE_STATION_CODE,
	invoice_num=:INVOICE_NUM,
	invoice_supplier=:INVOICE_SUPPLIER, invoice_supplier_code=:INVOICE_SUPPLIER_CODE,
	invoice_consign=:INVOICE_CONSIGN, invoice_consign_code=:INVOICE_CONSIGN_CODE,
	invoice_netto=:INVOICE_NETTO, invoice_tare=:INVOICE_TARE,
	disbalong=:DISBALONG, disbcross=:DISBCROSS, tol_disbalong=:TOL_DISBALONG, tol_disbcross=:TOL_DISBCROSS, data_sent=0
WHERE id=:ID