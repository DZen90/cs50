#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long long cc_number;
    do
    {
        printf("Number: ");
        cc_number = get_long_long();
    } while(cc_number <= 0);
    
    int length = 0;
    long long order = 10;
    long long max_order = order;
    int sum = 0;
    int digit = 0;
    bool even = false;
    while(cc_number % (order/10) != cc_number)
    {
        max_order = order;
        if (even)
        {
            digit = ((cc_number % order) - (cc_number % (order / 10))) / (order / 10) * 2;
            if (digit >= 10)
            {
                digit = (digit % 10) + digit / 10;
            }
        }
        else
        {
            digit = ((cc_number % order) - (cc_number % (order / 10))) / (order / 10);
        }
        sum += digit;
        even = !even;
        order *= 10;
        length++;
    }

    if (sum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }
    
    if (length == 16)
    {
        int first_two = ((cc_number % max_order) - (cc_number % (max_order/100))) / (max_order/100);
        if (first_two >= 51 && first_two <= 55)
        {
            printf("MASTERCARD\n");
            return 0;
        }
        else
        {
            int first = ((cc_number % max_order) - (cc_number % (max_order/10))) / (max_order/10);
            if (first == 4)
            {
                printf("VISA\n");
                return 0;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }
        }
    } 
    else if (length == 15)
    {
        int first_two = ((cc_number % max_order) - (cc_number % (max_order/100))) / (max_order/100);
        if (first_two == 34 || first_two == 37)
        {
            printf("AMEX\n");
            return 0;
        }
        else
        {
            printf("INVALID\n");
            return 0;
        }
    }
    else if (length == 13 || length == 16)
    {
        int first = ((cc_number % max_order) - (cc_number % (max_order/10))) / (max_order/10);
        if (first == 4)
        {
            printf("VISA\n");
            return 0;
        }
        else
        {
            printf("INVALID\n");
            return 0;
        }
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }
}