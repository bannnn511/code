-- NON-NULL -> count(column_name)

SELECT director, ARRAY_AGG(title)
FROM movies
GROUP BY director;

SELECT director, JSON_AGG(title)
FROM movies
GROUP BY director;

SELECT director, BOOL_OR(releaseyear > 1999) old_school
FROM movies
GROUP BY director;

SELECT director
FROM movies
GROUP BY director
HAVING BOOL_OR(releaseyear > 1999);


SELECT director, COUNT(*) FILTER ( WHERE releaseyear > 1999 )
FROM movies
GROUP BY director;

SELECT director, STRING_AGG(title, ',') FILTER ( WHERE releaseyear > 1999 )
FROM movies
GROUP BY director;


SELECT director, COUNT(*)
FROM movies
GROUP BY director;

