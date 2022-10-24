SELECT
    ifnull(CompanyName, "MISSING_NAME") as CompanyName,
    CustomerId,
    exp
FROM
    (
        SELECT
            CustomerId,
            NTILE(4) OVER (
                ORDER BY
                    res.exp
            ) as ntiles,
            exp
        FROM
            (
                SELECT
                    CustomerId,
                    round(sum(UnitPrice * Quantity), 2) as exp
                FROM
                    "order" as ord,
                    OrderDetail as od
                WHERE
                    ord.id = od.OrderId
                GROUP BY
                    CustomerId
            ) as res
    )
    LEFT OUTER JOIN Customer ON CustomerId = Customer.Id
WHERE
    ntiles = 1