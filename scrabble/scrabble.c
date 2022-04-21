#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
char LETTERS[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

int compute_score(string word);
int get_index(char o, char arr[], int arr_size);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);
    if (score1 > score2)
    {
        printf("Player 1 wins!");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!");
    }
    else
    {
        printf("Tie!");
    }
    return 0;
}

int compute_score(string word)
{
    int points = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        //If the character is either an uppercase letter or a lowercase letter
        if ((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z'))
        {
            //sums the point count related to that index
            points += POINTS[get_index(word[i], LETTERS, sizeof(LETTERS))];
        }
    }
    return points;
}

int get_index(char o, char arr[], int arr_size)
{
    //Iterates through the array and return the index of THE FIRST OCCURENCE of o
    for (int i = 0, n = arr_size; i < n; i++)
    {
        if ((int) toupper(arr[i]) == (int) toupper(o))
        {
            return i;
        }
    }
    return -1;
}
