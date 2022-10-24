SELECT
    id,
    shipcountry,
    CASE
        WHEN(shipcountry IN ("USA", "Mexico", "Canada")) THEN "NorthAmerica"
        ELSE "OtherPlace"
    END
FROM
    'Order'
Limit
    20 offset 15445 -10248;