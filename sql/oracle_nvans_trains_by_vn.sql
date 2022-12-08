SELECT TO_NCHAR(RWNUM) RWNUM, NDATETIME, COUNT(*) VAN_COUNT FROM (
	SELECT A.RWNUM, A.NDATETIME FROM (
		(SELECT RWNUM, NDATETIME FROM BPTL.RP_NVANS WHERE INVNUM=:INVNUM GROUP BY RWNUM, NDATETIME) A
		LEFT JOIN
		(SELECT RWNUM, NDATETIME, INVNUM FROM BPTL.RP_NVANS) B ON A.RWNUM = B.RWNUM AND A.NDATETIME = B.NDATETIME
	)
)
GROUP BY RWNUM, NDATETIME
ORDER BY NDATETIME DESC, RWNUM;