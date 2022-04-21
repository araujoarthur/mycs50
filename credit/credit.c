#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    long card_number;
    do
    {
        card_number = get_long("What's your CC number? ");
    }
    while (card_number <= 0); //Asks for card number until a positive number is given

    long card = card_number;
    int dcount = 0;

    // Counts card number digits by truncating int type when divided by 10.
    while (card_number > 0)
    {
        card_number = card_number / 10;
        dcount++;
    }

    card_number = card;

    //Checks digit count for valid digit counts.
    if (dcount != 16 && dcount != 15 && dcount != 13)
    {
        printf("INVALID\n");
    }
    else //If digits are not invalid, performs the checksum
    {
        int mult_digits = 0, non_mult = 0, digit, digit_sum;
        int card_digit = card_number / pow(10, dcount - 2);
        string card_type = "INVALID";
        // MC: 51, 52, 53, 54, or 55
        // AE: 34, 37
        // VISA: 4
        //printf("%i\n ", card_digit);
        while (card_number > 0)
        {
            digit = 0;
            digit_sum = 0;

            non_mult += card_number % 10;
            card_number = card_number / 10;

            digit = 2 * (card_number % 10);
            while (digit > 0)
            {
                digit_sum += (digit % 10);
                digit = digit / 10;
            }

            mult_digits += digit_sum;
            card_number = card_number / 10;

            if (card_digit <= 55 && card_digit >= 51)
            {
                card_type = "MASTERCARD";
            }
            else if (card_digit == 34 || card_digit == 37)
            {
                card_type = "AMEX";
            }
            else if (card_digit / 10 == 4)
            {
                card_type = "VISA";
            }
        }

        if ((mult_digits + non_mult) % 10 == 0)
        {
            printf("%s\n", card_type);
        }
        else
        {
            printf("INVALID\n");
        }
    }



}
