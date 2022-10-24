SELECT
    pname,
    comname,
    contname
FROM
    (
        SELECT
            p.ProductName as pname,
            ord.CustomerId as cid,
            min(ord.OrderDate) as date,
            cu.CompanyName as comname,
            cu.ContactName as contname
        FROM
            OrderDetail as od,
            (
                SELECT
                    *
                from
                    Product
                WHERE
                    Product.Discontinued = 1
            ) as p,
            "order" as ord,
            Customer as cu
        WHERE
            od.OrderId = ord.Id
            AND od.ProductId = p.Id
            AND cid = cu.id
        GROUP BY
            pname
    )