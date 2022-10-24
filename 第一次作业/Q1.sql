SELECT
    distinct ShipName,
    substr(ShipName, 1, instr(ShipName, '-') -1) as result
FROM
    'order'
WHERE
    ShipName like '%-%'
order by
    ShipName;