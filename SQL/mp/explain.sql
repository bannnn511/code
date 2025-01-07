EXPLAIN
SELECT *
FROM users
WHERE email < 'b';

CREATE INDEX user_email_idx ON users USING btree (email);
DROP INDEX user_email_idx;

-- seq scan -> read the entire table in physical order
-- bitmap index scan -> scan the index, produces a map, then read pages in physical order
-- index scan -> scan the index, get the rows
-- index only scan -> scan the index

EXPLAIN (ANALYSE , COSTS OFF )
SELECT *
FROM users
WHERE email < 'b'
LIMIT 10;

EXPLAIN (ANALYSE)
SELECT *
FROM users
WHERE email < 'b'
LIMIT 10;