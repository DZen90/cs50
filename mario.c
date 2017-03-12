#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    do
    {
        printf("Height: ");
        height = get_int();
    } while(height < 0 || height > 23);
    
    for (int i = 1; i <= height; i++)
    {
        int j = height;
        while(j > i)
        {
            printf(" ");
            j--;
        }
        while(j > 0)
        {
            printf("#");
            j--;
        }
        printf("  ");
        while(j < i)
        {
            printf("#");
            j++;
        }
        printf("\n");
    }
}