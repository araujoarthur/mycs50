#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


char ALPHABET[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};


int get_index(char o, char arr[], int arr_size);
string strupper(string stru);
int keycheck(string strc);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = strupper(argv[1]);

    int response = keycheck(key);
    if (response == -1 || response == -2)
    {
        // Prints a warning and exits if invalid charcount or invalid character
        printf("Key must contain 26 A-Z or a-z characters.\n");
        return 1;
    }
    else if (response == -3)
    {
        // Prints a warning and exits if there's duplicates
        printf("Key must contain 26 non-duplicate characters.\n");
        return 1;
    }

    string text = get_string("Text: ");

    // Makes the substitution
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            //If is uppercase just replace with the respective letter from key.
            text[i] = key[get_index(text[i], ALPHABET, sizeof(ALPHABET))];
        }
        else if (text[i] >= 'a' && text[i] <= 'z')
        {
            //If is not uppercase, runs tolower() before assign.
            text[i] = tolower(key[get_index(text[i], ALPHABET, sizeof(ALPHABET))]);
        }
    }

    printf("ciphertext: %s\n", text);

}

//Just copy pasted it from my scrabble solution
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

string strupper(string stru)
{
    //Receives a string and uppercases all its lower case characters (very useful for key standardization)
    for (int i = 0, n = strlen(stru); n > i; i++)
    {
        if (stru[i] >= 'a' && stru[i] <= 'z')
        {
            stru[i] -= 32;
        }
    }
    return stru;
}

int keycheck(string strc)
{
    //Keycheck returns -1 if length is wrong
    if (strlen(strc) != 26)
    {
        return -1;
    }
    //Returns -2 if invalid char is found
    for (int i = 0, n = strlen(strc); i < n; i++)
    {
        if (!((strc[i] >= 'a' && strc[i] <= 'z') || (strc[i] >= 'A' && strc[i] <= 'Z')))
        {
            return -2;
        }
        else
        {
            for (int m = i + 1; m < n; m++)
            {
                if (strc[i] == strc[m])
                {
                    // Returns -3 if duplicate is found
                    return -3;
                }
            }
        }
    }

    return 1;

}
