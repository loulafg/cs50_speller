// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"

static int dict_size = 0;

// Represents a node in a hash table that spreads words for search
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table, chose large prime for better distribution;
const unsigned int N = 200003; 

// Hash table
static struct node *table[N];

void free_list(struct node *p) //This funtion frees one by one the nodes in the linked list, from finish to beginning (recursively)
{
    if (p != NULL)
        free_list(p->next);   
    free(p);    
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char *wrd = malloc((LENGTH + 1) * sizeof(char));
    //copy the word here so as to get it all in lowercase
     
    for (int i = 0; i < strlen(word) + 1; i++)
    {
        wrd[i] = tolower(word[i]);
    }

    struct node *n; //this pointer traverses the list in search
    n = table[hash(wrd)];
    while (n)
    {
        int comp = strcmp(wrd, n->word);
        if (comp == 0) 
        {
            return true;
        }
        else if (comp > 0)
        {
            return false;
        }
        n = n->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    
    unsigned long long int hash_num = 1;
        for (int i = 0; i < 46; i++)
        {
            if (!(word[i])) break;
            hash_num = (hash_num * ((int)(word[i]) - 96) + word[i]);
        }
        return hash_num % N; // N as primes only, previous tests seem to distribute better the hash this way, may be just an impression
        
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    
    FILE *dict;
    dict = fopen(dictionary, "r");
    if (dict == NULL)
        return false;
    //first block opens dictinary file

    char word[LENGTH + 1]; //use this variable to get the word in lowercase
    while ((fscanf(dict, "%s", word) != EOF) )
    {       
        for (int i = 0; i < strlen(word) + 1; i++)
        {
            word[i] = tolower(word[i]);
        }
        struct node *tmp = malloc(sizeof(struct node));
        int hsh = hash(word); //call hash function
        dict_size++;   //increment on static registering size of dictionary, used by func size;
        
        //this builds the lists on the hash table
        strcpy((*tmp).word, word);
        (*tmp).next = table[hsh];
        table[hsh] = tmp;
    } 
    fclose(dict);
    free(dict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size; //output count obtained during load function
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    
    for (int i = 0; i < N; i++)
    {
        free_list(table[i]);
    }
    //unload calls a recursive function
    return true;
}
