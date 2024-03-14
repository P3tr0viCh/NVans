SELECT id, scales, num, bdatetime, invnum, carrying, brutto, tare, tare_t, tareindex, iscales_tare, idatetime_tare, netto,
	cargotype, cargotype_code,
	invoice_num, invoice_supplier, invoice_consign,
	depart_station, depart_station_code, purpose_station, purpose_station_code,
	invoice_netto, invoice_tare, disbalong, disbcross, tol_disbalong, tol_disbcross, status
FROM mvans
WHERE trnum=:TRAIN_NUM
ORDER BY num