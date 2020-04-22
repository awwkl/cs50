SELECT title FROM
movies JOIN ratings ON movies.id = ratings.movie_id
WHERE id in
(
SELECT movie_id FROM
people JOIN stars ON people.id = stars.person_id
WHERE name = 'Chadwick Boseman'
)
ORDER BY rating DESC
LIMIT 5