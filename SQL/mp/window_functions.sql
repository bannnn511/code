SELECT title, director, origin, COUNT(*) OVER (PARTITION BY origin) AS count_origin
FROM movies;

SELECT *,
       FIRST_VALUE(id) OVER (PARTITION BY user_id ORDER BY id ASC)  AS first_value,
       FIRST_VALUE(id) OVER (PARTITION BY user_id ORDER BY id DESC) AS last_value
FROM bookmarks
LIMIT 30;

-- FRAME
SELECT *,
       ROW_NUMBER() OVER (PARTITION BY user_id),
       FIRST_VALUE(id) OVER (PARTITION BY user_id ORDER BY id ASC) AS first_value,
       LAST_VALUE(id)
       OVER (PARTITION BY user_id ORDER BY id ASC ROWS
           BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING)            AS last_value,
       MAX(saved_on) OVER (PARTITION BY saved_on)
FROM bookmarks
LIMIT 30;

---
SELECT title, director, origin, COUNT(*) OVER (PARTITION BY origin) AS count_origin
FROM movies;

SELECT *,
       ROW_NUMBER() OVER user_bookmarks,
       FIRST_VALUE(id) OVER user_bookmarks AS first_value,
       FIRST_VALUE(id) OVER user_bookmarks AS last_value
FROM bookmarks
WINDOW user_bookmarks AS (PARTITION BY user_id ORDER BY id DESC)
LIMIT 30;

----- FRAME RANGE
SELECT *, MAX(created_at) OVER (ORDER BY created_at DESC RANGE BETWEEN '2 hour' PRECEDING AND '1 hour' FOLLOWING)
FROM users
LIMIT 30;