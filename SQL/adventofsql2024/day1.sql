SELECT c.name,
       toys.primary_wish,
       toys.backup_wish,
       toys.favorite_color,
       toys.color_count,
       CASE
           WHEN tc.difficulty_to_make = 1 THEN 'Simple Gift'
           WHEN tc.difficulty_to_make = 2 THEN 'Moderate Gift'
           ELSE 'Complex Gift'
           END AS gift_complexity,
       CASE
           WHEN tc.category = 'outdoor' THEN 'Outside Workshop'
           WHEN tc.category = 'educational' THEN 'Learning Workshop'
           ELSE 'General Workshop'
           END AS workshop_assignment
FROM children c
         JOIN (SELECT wl.child_id                              AS child_id,
                      wl.wishes ->> 'first_choice'::varchar    AS primary_wish,
                      wl.wishes ->> 'second_choice'::text      AS backup_wish,
                      wl.wishes -> 'colors' ->> 0              AS favorite_color,
                      JSON_ARRAY_LENGTH(wl.wishes -> 'colors') AS color_count
               FROM wish_lists wl) AS toys ON toys.child_id = c.child_id
         JOIN toy_catalogue tc ON toys.primary_wish = tc.toy_name
ORDER BY C.name
LIMIT 5;