SELECT id, scales, num, bdatetime, invnum, carrying, brutto, tare, tare_t, tareindex, iscales_tare, idatetime_tare, netto,
	cargotype, CAST(cargotype_code AS SIGNED) AS cargotype_code,
	invoice_num, invoice_supplier, invoice_consign, depart_station, purpose_station,
	invoice_netto, invoice_tare, disbalong, disbcross, tol_disbalong, tol_disbcross, status
FROM mvans
WHERE trnum=:TRAIN_NUM
ORDER BY num