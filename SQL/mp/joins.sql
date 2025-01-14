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
LIMIT 10;


-- ROWS FROM

SELECT lower, CHR(upper)
FROM ROWS FROM (
         GENERATE_SERIES(1, 10),
         GENERATE_SERIES(65, 75)
         ) AS t(lower, upper);

SELECT date::date, num
FROM ROWS FROM (
         GENERATE_SERIES('2024-01-01'::date, '2024-12-31'::date, '1 day'),
         GENERATE_SERIES(1, 366)
         ) AS t(date, num)


-- users that have > 16 bookmarks

EXPLAIN (ANALYZE, BUFFERS, VERBOSE, SETTINGS, FORMAT JSON)
SELECT *
FROM users
WHERE id IN (SELECT user_id
             FROM bookmarks
             GROUP BY user_id
             HAVING COUNT(*) > 16)



EXPLAIN
SELECT *
FROM users
WHERE EXISTS (SELECT 1
              FROM bookmarks
              WHERE users.id = bookmarks.user_id
              GROUP BY user_id
              HAVING COUNT(*) > 16);

EXPLAIN ANALYSE
SELECT *
FROM users
WHERE EXISTS (SELECT 1
              FROM bookmarks
              WHERE users.id = bookmarks.user_id
                AND starts_with(url, 'https')
              GROUP BY user_id
              HAVING COUNT(*) > 16);

EXPLAIN ANALYSE
SELECT users.id, users.first_name, users.last_name, whale_users.ct
FROM (SELECT user_id, COUNT(*) AS ct
      FROM bookmarks
      GROUP BY user_id
      HAVING COUNT(*) > 16) AS whale_users
         JOIN users ON users.id = whale_users.user_id;

EXPLAIN ANALYSE
SELECT *
FROM users
WHERE EXISTS (SELECT 1
              FROM bookmarks
              WHERE users.id = bookmarks.user_id
              GROUP BY user_id
              HAVING COUNT(*) > 16);



VACUUM ANALYZE bookmarks;
VACUUM FULL users;

SELECT COUNT(*)
FROM pg_stat_activity
WHERE query LIKE 'autovacuum:%';

















