// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <stdbool.h>

#include "dictionary.h"

// word count needed for size
int dict_word_count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 99999; // max 99999 buckets of linked list

// Hash table
node *table[N];

bool check_node(node *np, char *word) // word received has been converted to lowercase
{
    if (np == NULL) // np was passed in as table[hash_index] => NULL means word not found in dictionary
    {
        return false;
    }

    if (strcmp(np->word, word) == 0)
    {
        return true;
    }
    if (np->next == NULL) // reached end of linked list, no more next node
    {
        return false;
    }

    return check_node(np->next, word); // goto next node
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // make lowercase, then get hash value
    // access linked list at table[hash_index]

    char copy[LENGTH + 1];
    strcpy(copy, word); // copy word to avoid changing const char *word
    for (int i = 0; copy[i]; i++)
    {
        copy[i] = tolower(copy[i]);
    }

    int hash_index = hash(copy);
    //printf("checking %s - %i\n", copy, hash_index);

    if (check_node(table[hash_index], copy))
    {
        //printf("%s is a word!\n", copy);
        return true;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word) // used for placing words in table, AND checking
{
    int hash_value = 1;
    int iterate;
    if (strlen(word) < 4)
    {
        iterate = strlen(word);
    }
    else
    {
        iterate = 4;
    }

    for (int i = 0; i < iterate; i++) // can become > 450,000, but modulo cap below 99,999
    {
        hash_value *= ((int) word[i]);
    }

    return hash_value % N; // modulo limits number of hash table entries
}

bool place_in_table(int hash_index, node *np)
{
    if (table[hash_index] == NULL)
    {
        table[hash_index] = np;
    }
    else // insert node at start of linked list. for convenience
    {
        np->next = table[hash_index];
        table[hash_index] = np;
    }
    return true;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file
    // read strings one at a time getline(&line, &len, fp)
    // create a new node for each word
    // node *n = malloc(sizeof(node));
    // strcopy(n->word, "Hello");
    // n->next = NULL;
    // hash word to get uint
    // insert node into hash table at that location

    FILE *infile = fopen(dictionary, "r"); // dictionary is a string
    if (infile == NULL)
    {
        return false;
    }

    char word[46];
    while (fscanf(infile, "%s", word) != EOF)
    {
        dict_word_count++;

        node *n = calloc(1, sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word);
        int hash_index = hash(word); // returns 0 by default
        //printf("loading %s - %i\n", word, hash_index);
        place_in_table(hash_index, n); // insert at front of linked list
    }

    fclose(infile);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_word_count; // global var, updated during dictionary loading
}

void free_node(node *current) // current is table[i]
{
    node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i]) // if table[i] is valid node pointer
        {
            free_node(table[i]);
        }
    }

    return true;
}
