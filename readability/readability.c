#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    // Didn't use any extra function because it could be done in only one for loop (instead of three different functions)
    int words = 0, sentences = 0, letters = 0, grade;
    float sentences_per, letters_per, readability;
    string text = get_string("Text: ");
    for (int i = 0, text_len = strlen(text); i <= text_len; i++)
    {
        if (text[i] == 32 || text[i] == 0)
        {
            // If the current char is a space sums a word, or if t
            words += 1;
        }
        else if (text[i] == 46 || text[i] == 63 || text[i] == 33)
        {
            // If the current char is a period, question mark or exclamation, sums a sentence
            sentences += 1;
        }
        else if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            //If char is a letter, counts letters.
            letters += 1;
        }
    }
    // Make some calculations
    sentences_per = sentences * 100.0 / words;
    letters_per = letters * 100.0 / words;
    readability = 0.0588 * letters_per - 0.296 * sentences_per - 15.8;
    grade = (int) round(readability);

    if (grade <= 0)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 0 && grade < 16)
    {
        printf("Grade %i\n", grade);
    }
    else
    {
        printf("Grade 16+\n");
    }
}

