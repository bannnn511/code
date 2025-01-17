WITH all_user AS (SELECT FALSE AS is_deleted, *
                  FROM users
                  UNION ALL
                  SELECT TRUE AS is_deleted, *
                  FROM users_archive),
     aaron AS (SELECT * FROM all_user WHERE email = 'aaron.francis@example.com')

SELECT *
FROM aaron;
