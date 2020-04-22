SELECT name
FROM directors JOIN people ON directors.person_id = people.id
WHERE movie_id in
(SELECT id FROM
movies JOIN ratings ON movies.id = ratings.movie_id
WHERE rating >= 9)