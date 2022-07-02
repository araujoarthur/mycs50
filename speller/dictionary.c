// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

#define ALPHABET 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
//const unsigned int N = 46;
//const unsigned int NN = 26;


unsigned int TOTAL_WORD_COUNT = 0;

// Hash table
node *table[ALPHABET][LENGTH + 1];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char upperWord[LENGTH + 1];
    strcpy(upperWord, word);
    string_upper(upperWord);
    unsigned int wrd_hash = hash(upperWord);
    unsigned int wrd_len = strlen(upperWord);
    for (node *tmp = table[wrd_hash][wrd_len]; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(upperWord, tmp->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function || Really needed? :T
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    char word[LENGTH + 1];
    FILE *dict = fopen(dictionary, "r");

    if (dict == NULL)
    {
        return false;
    }

    while (fscanf(dict, "%s", word) != EOF)
    {
        node *wrd = malloc(sizeof(node));
        if (wrd == NULL)
        {
            return false;
        }
        int hashv = hash(word);
        int len = strlen(word);

        string_upper(word);

        strcpy(wrd->word, word);
        wrd->next = table[hashv][len];
        table[hashv][len] = wrd;
        TOTAL_WORD_COUNT++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return TOTAL_WORD_COUNT;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < ALPHABET; i++)
    {
        for (int j = 0; j < LENGTH + 1; j++)
        {
            //Used for loop bc it looks smarter ;)
            for (node *tmp = table[i][j]; tmp != NULL;)
            {
                node *holder = tmp;
                tmp = tmp->next;
                free(holder);
            }
        }
    }
    return true;
}

//Not the most efficient one, stopped developing it as it worked for my objectives
void string_upper(char *word)
{
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        if (isalpha(word[i]) && !(word[i] >= 41 && word[i] <= 90))
        {
            word[i] = word[i] - 32;
        }
    }
}

void __debugDict(void)
{
    for (int i = 0; i < ALPHABET; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            for (node *tmp = table[i][j]; tmp != NULL; tmp = tmp->next)
            {
                printf("%s\n", tmp->word);
            }
            continue;
        }
    }

}