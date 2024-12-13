SELECT STRING_AGG(CHR(src.value), '')
FROM (SELECT a.id, a.value
      FROM letters_a a
      WHERE (value >= 65 AND value <= 90)
         OR (value >= 97 AND value <= 122)
         OR (value >= 32 AND value <= 34)
         OR value IN (32, 44, 33)
      UNION ALL
      SELECT b.id, b.value
      FROM letters_b b
      WHERE (value >= 65 AND value <= 90)
         OR (value >= 97 AND value <= 122)
         OR (value >= 32 AND value <= 34)
         OR value IN (32, 44, 33)) src;
