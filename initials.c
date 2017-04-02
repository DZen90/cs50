#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // Input string
    string s = get_string();
    
    if (s != NULL)
    {
        int n = strlen(s);
        bool isfirst = true;
        
        // Iterate over the characters in the string
        for (int i = 0; i < n; i++)
        {
            // Print the character in the upper case if it is not a space and the previous character was a space
            if (s[i] == ' ' || s[i] == '-')
            {
                isfirst = true;
            }
            else
            {
                if (isfirst)
                {
                    printf("%c", toupper(s[i]));
                    isfirst = false;
                }
            }
        }
        printf("\n");
    }
}