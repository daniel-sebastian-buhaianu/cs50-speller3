// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Number of words in dictionary
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char lower_word[LENGTH + 1];
    int i = 0;
    while (word[i] != '\0')
    {
        lower_word[i] = tolower(word[i]);
        i++;
    }
    lower_word[i] = '\0';
    
    unsigned int index = hash(lower_word);

    for (node *ptr = table[index]; ptr != NULL; ptr = ptr->next)
    {
        if (strcmp(ptr->word, lower_word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash_value = 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        char c = tolower(word[i]);
        hash_value = hash_value * 31 + c;
    }

    return hash_value % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];
    while(fscanf(file, "%s", word) != EOF)
    {
        word_count++;

        node *new_node = malloc(sizeof(node));
        if (new_node != NULL)
        {
            strcpy(new_node->word, word);

            unsigned int index = hash(word);
            new_node->next = table[index];
            table[index] = new_node;
        }
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            node *tmp = ptr;
            ptr = ptr->next;
            free(tmp);
        }
    }

    return true;
}
