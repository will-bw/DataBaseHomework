SELECT
    CategoryName,
    count(*) as cnt,
    round(avg(UnitPrice), 2) as average,
    min(UnitPrice) as mini,
    max(UnitPrice) as maxi,
    sum(UnitsOnOrder) as sum
FROM
    Category as c,
    Product
WHERE
    CategoryId = c.Id
    
GROUP BY
    c.Id
HAVING
    sum > 10