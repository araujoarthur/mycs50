-- write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
-- Your query should output a table with a single column for the name of each person.
-- There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
-- Kevin Bacon himself should not be included in the resulting list.

SELECT DISTINCT name FROM people
    INNER JOIN stars ON stars.person_id = people.id
    WHERE stars.movie_id IN (SELECT movies.id FROM movies
                            INNER JOIN stars ON movies.id = stars.movie_id
                            INNER JOIN people ON stars.person_id = people.id
                            WHERE people.name = "Kevin Bacon") AND NOT people.name = "Kevin Bacon" ORDER BY people.name

