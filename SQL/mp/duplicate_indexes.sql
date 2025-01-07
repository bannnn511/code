CREATE INDEX email ON users (email);
CREATE INDEX email_is_pro ON users (email, is_pro);

DROP INDEX email;

EXPLAIN
SELECT *
FROM users
WHERE email = 'aaron.francis@example.com';