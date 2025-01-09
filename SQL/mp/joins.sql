-- CROSS  JOINS
SELECT *
FROM GENERATE_SERIES(0, 10);

SELECT CHR(l)
FROM GENERATE_SERIES(65, 75) AS letters(l);

SELECT (CHR(l) || n) AS code
FROM GENERATE_SERIES(0, 10) AS numbers(n),
     GENERATE_SERIES(65, 75) AS letters(l);

-- INNER JOINs
SELECT users.id, users.first_name, bookmarks.url
FROM bookmarks
         JOIN users ON bookmarks.user_id = users.id
LIMIT 10

-- sub queries
CREATE INDEX bookmarks_secure_url ON bookmarks (user_id, (starts_with(url, 'https')));

DROP INDEX bookmarks_secure_url;

EXPLAIN
SELECT *
FROM bookmarks
WHERE (starts_with(url, 'https'));

EXPLAIN ANALYSE
SELECT first_name, url, bm.id
FROM users
         JOIN (SELECT *
               FROM bookmarks
               WHERE starts_with(url, 'https') IS TRUE) bm ON users.id = bm.user_id
LIMIT 100;

-- lateral

EXPLAIN
SELECT u.id, u.first_name, most_recent_bookmarks.id, most_recent_bookmarks.url
FROM users u
         JOIN LATERAL (SELECT *
                       FROM bookmarks
                       WHERE user_id = u.id
                       ORDER BY id DESC
                       LIMIT 1) most_recent_bookmarks ON TRUE
LIMIT 10