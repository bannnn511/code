DROP TABLE real_example;
CREATE TABLE real_example
(
    sensor_name text,
    reading     float8
);

SELECT 7.0::float8 * (2.0 / 10.0);

SELECT SUM(num::float8 / (num + 1))
FROM GENERATE_SERIES(1, 20000000) num;

SELECT SUM(num::numeric / (num + 1))
FROM GENERATE_SERIES(1, 20000000) num;

SELECT *
FROM GENERATE_SERIES(1, 20) num;

SELECT PG_TYPEOF(100::money
       );

SELECT PG_TYPEOF(integer '100');


-- BOOLEAN
DROP TABLE boolean_example;
CREATE TABLE boolean_example
(
    status BOOLEAN
);

INSERT INTO boolean_example (status)
VALUES (TRUE),
       (FALSE),
       ('t'),
       ('true'),
       ('f'),
       ('false'),
       ('1'),
       ('0'),
       ('on'),
       ('off');

SELECT *
FROM boolean_example;

-- TIMESTAMPTZ
CREATE TABLE timestamp_examples
(
    "id"        serial PRIMARY KEY,
    "timestamp" timestamptz
);

-- ISO 8601
-- 2024-01-31 11:30:08
SHOW DATESTYLE;

SELECT '1/3/2024'::date;

SELECT NOW()::timestamptz,
       NOW()::timestamptz(0),
       NOW()::timestamptz(1);

SELECT TO_TIMESTAMP(1694793600);

-- TIMEZONE
SHOW TIMEZONE;

ALTER DATABASE postgres SET time ZONE 'UTC';

SELECT '2024-01-31 11:30:08'::timestamptz,
       '2024-01-31 11:30:08'::timestamptz AT TIME ZONE 'America/Chicago',
       '2024-01-31 11:30:08'::timestamptz AT TIME ZONE 'CST',
       '2024-01-31 11:30:08'::timestamptz AT TIME ZONE '-06:00' AS hour_offset, -- wrong
       '2024-01-31 11:30:08'::timestamptz AT TIME ZONE '06:00'  AS hour_offset;

SELECT *
FROM pg_timezone_names
WHERE name ILIKE '%chicago%';

-- yesterday
SELECT 'yesterday'::timestamptz;
SELECT CURRENT_DATE - 1;


DROP INDEX users_birthday_idx;

CREATE INDEX multi ON users USING btree (first_name, last_name, birthday);
CREATE INDEX multi2 ON users USING btree (birthday, first_name, last_name);

DROP INDEX multi;
DROP INDEX multi2;


EXPLAIN
SELECT *
FROM users
WHERE TRUE
  AND birthday < '1989-02-21'
  AND first_name = 'Aaron'
  AND last_name = 'Francis';

CREATE INDEX "first" ON users USING btree (first_name);
CREATE INDEX "last" ON users USING btree (last_name);
CREATE INDEX "first_last" ON users USING btree (first_name, last_name);

DROP INDEX "first";
DROP INDEX "last";
DROP INDEX "first_last";

EXPLAIN
SELECT *
FROM users
WHERE first_name = 'Aaron'
  AND last_name = 'Francis';

EXPLAIN
SELECT *
FROM users
WHERE first_name = 'Aaron'
   OR last_name = 'Francis';

-- index only scan
CREATE INDEX "multi2" ON users (first_name, last_name) INCLUDE (id);

EXPLAIN
SELECT first_name
FROM users
WHERE first_name = 'Aaron';

SELECT *
FROM pg_indexes
WHERE tablename = 'users';

DROP INDEX birthday_created;
CREATE INDEX birthday_created ON users (birthday DESC, created_at ASC);

EXPLAIN
SELECT *
FROM users
ORDER BY birthday ASC, created_at DESC
LIMIT 10;

-- UNION
SELECT TRUE AS active, *
FROM users
WHERE email = 'aaron.francis@example.com'
UNION
SELECT FALSE, *
FROM users_archive
WHERE email = 'aaron.francis@example.com';

SELECT 1
UNION ALL
SELECT 1;

-- GENERATE SERIES and ordinality
SELECT *
FROM UNNEST(ARRAY ['first', 'second', 'third']) WITH ORDINALITY AS t(element, ordinality);

-- jsonb to record
SELECT *
FROM JSONB_TO_RECORDSET('[
  {
    "id": 1,
    "name": "an"
  }
]'::jsonb) AS x (id int, name text);



























