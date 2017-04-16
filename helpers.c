/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

// upper limit on range of integers that can be generated
#define LIMIT 65536

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // Value must be positive
    if (value <= 0)
    {
        return false;
    }
    
    int lower = 0;
    int upper = n - 1;
    int pivot = n / 2;
    
    // Check if the value is within the max and min of values
    if (value < values[lower] || value > values[upper])
    {
        return false;
    }
    
    if (value == values[lower] || value == values[upper])
    {
        return true;
    }
    
    // Search while searching range is not empty
    while(upper - lower > 1)
    {
        // Calculate the middle of the searching range
        pivot = lower + (upper - lower + 1) / 2;
        
        // Check the lower, the upper and the middle element
        if (value == values[lower] || value == values[upper] || value == values[pivot])
        {
            return true;
        }
        
        // Reduce the searching range
        if (value < values[pivot])
        {
            upper = pivot;
        }
        else
        {
            lower = pivot;
        }
        
    }
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    int counts[LIMIT];
    
    for (int i = 0; i < n; i++)
    {
        counts[values[i]]++;
    }
    
    // Fill the array with sorted values
    int k = 0;
    for (int i = 0; i < n; i++)
    {
        while (counts[k] == 0)
        {
            k++;
        }
        values[i] = k;
        k++;
    }
    
    return;
}
