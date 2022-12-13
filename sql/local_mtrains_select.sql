SELECT id, trnum, bdatetime, nvans, status
FROM mtrains
WHERE bdatetime>=:DATE_FROM AND bdatetime<:DATE_TO
ORDER BY bdatetime DESC 