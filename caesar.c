#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Check for command line arguments
    if (argc != 2) {
        printf("Invalid arguments\n");
        return 1;
    }
    
    int key = atoi(argv[1]);
    if (key < 0) {
        printf("Invalid arguments\n");
        return 1;
    }

    
    // Get user input
    printf("plaintext: ");
    string s = get_string();
    
    printf("ciphertext: ");
    
    // Iterate over the characters
    int n = strlen(s);
    for (int i = 0; i < n; i++)
    {
        char c = s[i];
        if (isalpha(c))
        {
            // Rotate the character by the key preserving its case
            if (islower(c))
            {
                c = (c + key - 'a') % 26 + 'a';
            }
            else if (isupper(c))
            {
                c = (c + key - 'A') % 26 + 'A';
            }
        }
        printf("%c", c);
    }
    printf("\n");

}