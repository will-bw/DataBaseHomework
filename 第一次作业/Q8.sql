SELECT
    Region.RegionDescription,
    Employee.FirstName,
    Employee.LastName,
    max(Employee.BirthDate) as BirthDate
FROM
    EmployeeTerritory,
    Employee,
    Territory,
    Region
WHERE
    EmployeeTerritory.EmployeeId = Employee.Id
    and Territory.RegionId = Region.Id
    and EmployeeTerritory.TerritoryId = Territory.Id
GROUP BY
    Region.Id