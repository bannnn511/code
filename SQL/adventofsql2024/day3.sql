SELECT UNNEST(food_items) AS items, COUNT(*) AS freq
FROM (SELECT CASE
                 WHEN XMLEXISTS('//total_guests/text()' PASSING BY REF menu_data) = 'true'
                     THEN (XPATH('//total_guests/text()', menu_data))[1]::text::integer
                 WHEN XMLEXISTS('//total_count/text()' PASSING BY REF menu_data) = 'true'
                     THEN (XPATH('//total_count/text()', menu_data))[1]::text::integer
                 WHEN XMLEXISTS('//guestCount/text()' PASSING BY REF menu_data) = 'true'
                     THEN (XPATH('//guestCount/text()', menu_data))[1]::text::integer
                 ELSE 0
                 END                                              AS guest_count,
             XPATH('//food_item_id/text()', menu_data)::varchar[] AS food_items
      FROM christmas_menus)
WHERE guest_count > 78
GROUP BY items
ORDER BY freq DESC
LIMIT 5

