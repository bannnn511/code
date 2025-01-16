SELECT origin, genre, COUNT(*)
FROM movies
GROUP BY
    GROUPING SETS ( origin, genre
    );

----------------
SELECT origin, genre, COUNT(*)
FROM movies
GROUP BY
    ROLLUP (origin, genre);
--
SELECT origin, genre, COUNT(*)
FROM movies
GROUP BY
    GROUPING SETS ( (origin, genre), (origin), ()
    );
----------------

SELECT origin, genre, COUNT(*)
FROM movies
GROUP BY
    CUBE (origin, genre);

SELECT origin, genre, COUNT(*)
FROM movies
GROUP BY
    GROUPING SETS ( (origin, genre),
                    (origin),
                    (genre),
    ()
    );