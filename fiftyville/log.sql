-- Keep a log of any SQL queries you execute as you solve the mystery.

-- 1) Opening reports:
SELECT * FROM crime_scene_reports WHERE day = 28 AND year = 2021 AND month  = 7;

-- We discover it happened around 10:15am at the bakery and there were interviews conducted.

-- 2) Checking on Interviews.

SELECT * FROM interviews WHERE year = 2021 AND month = 7 AND day = 28;

-- Ruth testimonial:
-- "Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame."

-- Eugene testimonial:
-- "I don't know the thief's name, but it was someone I recognized. Earlier this morning,
-- before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money."

-- Raymond testimonial:
-- "As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call,
-- I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket."

-- a) So now we know that sometime between 10:15-10:25 the thief left the Bakery in a car. Check plates between 10:15-10:25 for exits.
-- b) We know the thieve was withdrawing money on 28/07/2021 before the theft (So check for withdrawals before 10:15)
-- c) We also know the thief asked someone to buy its ticket on the EARLIEST flight out of Fiftyville on 29/07/2021 (So check for flights on the next day).
-- d) It's known that the thief asked it by phone, so we can catch the accomplice by looking in phone logs.

-- 3) Checking Plates on the given timeframe in a):

SELECT * FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND (minute >= 15
AND minute <= 25);

-- We then find 8 exits and can link it to people.

SELECT *
    FROM bakery_security_logs
    INNER JOIN people
        ON people.license_plate = bakery_security_logs.license_plate
    WHERE
            day = 28
        AND
            month = 7
        AND
            year = 2021
        AND
            hour = 10
        AND
            (minute >= 15 AND minute <= 25);

-- This query will return people who were on parking lot at that given time.
-- 3.1) We can now check for people withdrawing money in that morning as given in b)

SELECT * FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street";

-- We can Inner join that with bank accounts and people to get names:

SELECT people.id, people.passport_number, people.name, people.phone_number
    FROM atm_transactions
    INNER JOIN bank_accounts
        ON bank_accounts.account_number = atm_transactions.account_number
    INNER JOIN people
        ON people.id = bank_accounts.person_id
    WHERE
        day = 28
        AND
        month = 7
        AND
        year = 2021
        AND
        atm_location = "Leggett Street"
        AND
        people.id IN (SELECT people.id
                        FROM bakery_security_logs
                        INNER JOIN people
                            ON people.license_plate = bakery_security_logs.license_plate
                        WHERE
                                day = 28
                            AND
                                month = 7
                            AND
                                year = 2021
                            AND
                                hour = 10
                            AND
                                (minute >= 15 AND minute <= 25));

-- 3.2) Now we have only 4 suspects. We can then check for flights on the next days where their passport number is involved.

SELECT people.id, people.passport_number, people.name, people.phone_number, flights.hour, flights.minute
    FROM atm_transactions
    INNER JOIN bank_accounts
        ON bank_accounts.account_number = atm_transactions.account_number
    INNER JOIN people
        ON people.id = bank_accounts.person_id
    INNER JOIN passengers
        ON passengers.passport_number = people.passport_number
    INNER JOIN flights
        ON passengers.flight_id = flights.id
    INNER JOIN airports
    WHERE
        atm_transactions.day = 28
        AND
        atm_transactions.month = 7
        AND
        atm_transactions.year = 2021
        AND
        atm_transactions.atm_location = "Leggett Street"
        AND
        people.id IN (SELECT people.id
                        FROM bakery_security_logs
                        INNER JOIN people
                            ON people.license_plate = bakery_security_logs.license_plate
                        WHERE
                                day = 28
                            AND
                                month = 7
                            AND
                                year = 2021
                            AND
                                hour = 10
                            AND
                                (minute >= 15 AND minute <= 25))
        AND
        flights.day = 29
        AND
        flights.month = 7
        AND
        flights.year = 2021;


-- Then we verify for phone calls to check who made calls that day with duration below 60 seconds and links it to the earliest flight taken in that list (which returns Bruce)
-- Also we can join the airports so we can relate the destination id to the airport city.

SELECT people.name, flights.hour, flights.minute, airports.city FROM phone_calls
    INNER JOIN people
        ON people.phone_number = phone_calls.caller
    INNER JOIN passengers
        ON passengers.passport_number = people.passport_number
    INNER JOIN flights
        ON passengers.flight_id = flights.id
    INNER JOIN airports
        ON airports.id = flights.destination_airport_id
    WHERE
    flights.day = 29
    AND
    flights.month = 7
    AND
    flights.year = 2021
    AND
    phone_calls.duration < 60
    AND
    phone_calls.day = 28
    AND
    phone_calls.month = 7
    AND
    phone_calls.year = 2021
    AND
    phone_calls.caller IN (SELECT people.phone_number
                            FROM atm_transactions
                            INNER JOIN bank_accounts
                                ON bank_accounts.account_number = atm_transactions.account_number
                            INNER JOIN people
                                ON people.id = bank_accounts.person_id
                            INNER JOIN passengers
                                ON passengers.passport_number = people.passport_number
                            INNER JOIN flights
                                ON passengers.flight_id = flights.id
                            INNER JOIN airports
                            WHERE
                                atm_transactions.day = 28
                                AND
                                atm_transactions.month = 7
                                AND
                                atm_transactions.year = 2021
                                AND
                                atm_transactions.atm_location = "Leggett Street"
                                AND
                                people.id IN (SELECT people.id
                                                FROM bakery_security_logs
                                                INNER JOIN people
                                                    ON people.license_plate = bakery_security_logs.license_plate
                                                WHERE
                                                        day = 28
                                                    AND
                                                        month = 7
                                                    AND
                                                        year = 2021
                                                    AND
                                                        hour = 10
                                                    AND
                                                        (minute >= 15 AND minute <= 25))
                                AND
                                flights.day = 29
                                AND
                                flights.month = 7
                                AND
                                flights.year = 2021)
    ORDER BY flights.hour ASC LIMIT 1;

    -- A small change to the above query will reveal the accomplice name:
    -- First change: remove passengers, flights and airports joins and change phone_calls.caller to phone_calls.receiver (so we know who was bruce talking to.)
    -- Also, remove ordering by flight time and flights and airports references in select line.

    SELECT people.name, phone_calls.receiver, phone_calls.caller FROM phone_calls
    INNER JOIN people
        ON people.phone_number = phone_calls.receiver -- Here's the change.
    WHERE
    phone_calls.duration < 60
    AND
    phone_calls.day = 28
    AND
    phone_calls.month = 7
    AND
    phone_calls.year = 2021
    AND
    phone_calls.caller IN (SELECT people.phone_number
                            FROM atm_transactions
                            INNER JOIN bank_accounts
                                ON bank_accounts.account_number = atm_transactions.account_number
                            INNER JOIN people
                                ON people.id = bank_accounts.person_id
                            INNER JOIN passengers
                                ON passengers.passport_number = people.passport_number
                            INNER JOIN flights
                                ON passengers.flight_id = flights.id
                            INNER JOIN airports
                            WHERE
                                atm_transactions.day = 28
                                AND
                                atm_transactions.month = 7
                                AND
                                atm_transactions.year = 2021
                                AND
                                atm_transactions.atm_location = "Leggett Street"
                                AND
                                people.id IN (SELECT people.id
                                                FROM bakery_security_logs
                                                INNER JOIN people
                                                    ON people.license_plate = bakery_security_logs.license_plate
                                                WHERE
                                                        day = 28
                                                    AND
                                                        month = 7
                                                    AND
                                                        year = 2021
                                                    AND
                                                        hour = 10
                                                    AND
                                                        (minute >= 15 AND minute <= 25))
                                AND
                                flights.day = 29
                                AND
                                flights.month = 7
                                AND
                                flights.year = 2021);


-- So Bruce stole it, flew to New York City with support from Robin.

