SELECT NDATETIME FROM BPTL.RP_NVANS
WHERE RWNUM=:RWNUM
GROUP BY NDATETIME
ORDER BY NDATETIME DESC;