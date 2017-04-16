/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // Check command-line arguments. The programm shuold be run with 2 or 3 arguments
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // Get an integer from the second command-line argument
    int n = atoi(argv[1]);

    // Initializing buffer value, 
    // using the third argument (if specified) or current time, converted to long
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else
    {
        srand48((long) time(NULL));
    }

    /* Generate requested numbers 
    * of pseudorandom nonnegative double-precision floating-point values uniformly distributed between [0.0, 1.0)
    * then convert them to uniformly distributed between [0, 65536) integers by multiplying by 65536 (limit for integers)
    * and print them
    */
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
