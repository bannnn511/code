WITH all_user AS (SELECT FALSE AS is_deleted, *
                  FROM users
                  UNION ALL
                  SELECT TRUE AS is_deleted, *
                  FROM users_archive),
     aaron AS (SELECT * FROM all_user WHERE email = 'aaron.francis@example.com')

SELECT *
FROM aaron;

WITH ranked_bookmarks AS (SELECT *, ROW_NUMBER() OVER (PARTITION BY user_id ORDER BY id) AS num
                          FROM bookmarks)
SELECT *
FROM ranked_bookmarks
WHERE num <= 3
LIMIT 30;


WITH user_bookmarks AS (SELECT *,
                               LAG(id) OVER user_bookmarks IS NULL  AS is_first_bookmark,
                               LEAD(id) OVER user_bookmarks IS NULL AS is_last_bookmark
                        FROM bookmarks
                        WINDOW user_bookmarks AS (PARTITION BY user_id ORDER BY id))
SELECT *
FROM user_bookmarks
WHERE is_first_bookmark IS TRUE
   OR is_last_bookmark IS TRUE;

---
WITH RECURSIVE numbers AS (SELECT 1 AS n
                           UNION ALL
                           SELECT n + 1
                           FROM numbers
                           WHERE n < 10)
SELECT *
FROM numbers

-- Fibonacci
WITH RECURSIVE numbers (id, a, b) AS (SELECT 1, 0, 1 AS n
                                      UNION ALL
                                      SELECT id + 1, b, a + b
                                      FROM numbers
                                      WHERE id < 20)
SELECT id, a AS fib
FROM numbers;


---
WITH RECURSIVE all_categories AS (SELECT id, name, name AS path
                                  FROM categories
                                  WHERE parent_id IS NULL
                                  UNION ALL
                                  SELECT categories.id,
                                         categories.name,
                                         CONCAT(path, '->', categories.name)
                                  FROM all_categories
                                           INNER JOIN categories ON all_categories.id = categories.parent_id)
SELECT *
FROM all_categories
LIMIT 30;


SELECT *
FROM categories
WHERE parent_id IS NULL