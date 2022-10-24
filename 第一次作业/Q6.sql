SELECT
    orderId,
    orderDate,
    lag(OrderDate, 1, OrderDate) OVER(
        Order BY
            OrderDate
    ) as Previous,
    round(
        julianday((orderDate)) - julianday(
            lag(OrderDate, 1, OrderDate) OVER(
                Order BY
                    OrderDate
            )
        ),
        2
    ) as Differ
FROM
    (
        SELECT
            o.Id as orderId,
            o.OrderDate as orderDate
        FROM
            "Order" as o
        WHERE
            o.CustomerId = 'BLONP'
        Order BY
            OrderDate
    ) as ord
Order BY
    OrderDate
LIMIT
    10