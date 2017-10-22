/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"
#include <stdio.h>

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    node *current_node = root;
    
    while (*word != '\0')
    {
        if (isalpha(*word))
        {
            current_node = current_node->children[tolower(*word) - 'a'];
        }
        else
        {
            current_node = current_node->children[26];
        }
        if (current_node == NULL)
        {
            return false;
        }
        word++;
    }
    if (current_node->is_word)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // Open file
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    
    root = malloc(sizeof(node));
    root->is_word = false;
    for (int i = 0; i < 27; i++)
    {
        root->children[i] = NULL;
    }
    
    if (root == NULL)
    {
        return false;
    }
    
    node *current_node = root;

    // iterate over the characters in dictionary
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        if (isalpha(c))
        {
            if (current_node->children[c - 'a'] == NULL)
            {
                node *new_node = malloc(sizeof(node)); // Create next node
                new_node->is_word = false;
                for (int i = 0; i < 27; i++)
                {
                    new_node->children[i] = NULL;
                }

                current_node->children[c - 'a'] = new_node; // Assign it to chilren
                current_node = new_node; // Go to new node
            }
            else
            {
                current_node = current_node->children[c - 'a']; // Child node exists, go to it 
            }
        }
        else if (c == '\'')
        {
            if (current_node->children[26] == NULL)
            {
                node *new_node = malloc(sizeof(node)); // Create next node
                new_node->is_word = false;
                for (int i = 0; i < 27; i++)
                {
                    new_node->children[i] = NULL;
                }
                current_node->children[26] = new_node; // Assign it to chilren
                current_node = new_node; // Go to new node
            }
            else
            {
                current_node = current_node->children[26]; // Child node exists, go to it 
            }
        }
        else if (c == '\n')
        {
            // End of the word
            current_node->is_word = true;
            current_node = root;
        }
    }
    
    fclose(fp);

    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (root == NULL)
    {
        return 0;
    }
    
    int size = 0;
    node_size(root, &size);
    
    return size;
}

/**
 * Count number of words in particular node.
 */
void node_size(node *current_node, int *size_ptr)
{
    if (current_node == NULL)
    {
        return;
    }
    
    if (current_node->is_word)
    {
        (*size_ptr)++;
    }
    // Iterate over children and count words in them
    for (int i = 0; i < 27; i++)
    {
        node_size(current_node->children[i], size_ptr);
    }
    
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    unload_node(root);
    root = NULL;
    return true;
}

/**
 * Unloads particular node.
 */
void unload_node(node *current_node)
{
    if (current_node == NULL)
    {
        return;
    }
    // Iterate over children and unload them
    for (int i = 0; i < 27; i++)
    {
        unload_node(current_node->children[i]);
    }
    free(current_node);
}