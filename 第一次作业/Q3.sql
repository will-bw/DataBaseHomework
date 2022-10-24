SELECT
	com,
	ROUND(r1 * 100.00 / r2 * 1.00, 2) AS ratio
FROM
	(
		SELECT
			CompanyName AS com,
			COUNT(*) AS r1
		FROM
			"Order",
			Shipper
		WHERE
			ShippedDate > RequiredDate
			AND SHIPVia = Shipper.id
		GROUP BY
			Shipper.id
	),
	(
		SELECT
			CompanyName AS com1,
			COUNT(*) AS r2
		FROM
			"Order",
			Shipper
		WHERE
			SHIPVia = Shipper.id
		GROUP BY
			Shipper.id
	)
WHERE
	com = com1
Order By
	ratio DESC;