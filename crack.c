#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cs50.h>

string hash;

char key[5];

char salt[3];

// Recursively generate salt and check if hashes match
bool check_crypt()
{
    
    salt[0] = hash[0];
    salt[1] = hash[1];
    salt[2] = '\0';
    
    string genetated_hash = crypt(key, salt);
    //printf("%s %s\n", key, salt);
    if (strcmp(hash, genetated_hash) == 0)
    {
        printf("%s\n", key);
        return true;
    }
    else
    {
        return false;
    }

}
bool generate_key(int position)
{
    if (key[position] == '\0')
    {
        return check_crypt();
    }
    
    // letters in lower case
    while (key[position] <= 'z')
    {
        if (generate_key(position + 1))
        {
            return true;
        }
        key[position]++;
    }
    
    // letters in upper case
    key[position] = 'A';
    while (key[position] <= 'Z')
    {
        if (generate_key(position + 1))
        {
            return true;
        }
        key[position]++;
    }
    
    key[position] = 'a';
    return false;

}

int main(int argc, string argv[])
{
    // Check command line arguments
    if (argc != 2)
    {
        printf("Invalid arguments\n");
        return 1;
    }
    
    hash = argv[1];

    // Iterate over the characters in key
    bool isfound = false;
    for (int i = 0; i < 5; i++)
    {
        key[i] = 'a';
        isfound = generate_key(0);
        if (isfound)
        {
            return 0;
        }
    }
    
    if (!isfound)
    {
        printf("No matches\n");
    }

}