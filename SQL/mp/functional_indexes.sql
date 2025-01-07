DROP INDEX domain;
CREATE INDEX domain ON users ((SPLIT_PART(email, '@', 2)));

-- will use index
EXPLAIN
SELECT email
FROM users
WHERE SPLIT_PART(email, '@', 2) = 'beer.com'
LIMIT 10;

-- will not use index
EXPLAIN
SELECT email
FROM users
WHERE SPLIT_PART(email, '@', 1) = 'beer.com'
LIMIT 10;

CREATE INDEX email_lower ON users (LOWER(email));


SELECT email
FROM users
WHERE LOWER(email) = LOWER('AAron');