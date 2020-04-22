#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int multiply_digit_by_2(int n) // multiply by 2, add product's digits
{
    int new = n * 2;
    return (new / 10) + (new % 10); // works for new < 10 as well as n >= 10
}

int passed_checksum(long credit) // check if credit number passes checksum
{
    int total = 0;
    int last_digit = 0;
    while (credit > 0) // keep dividing by 10 to check last digit
    {
        last_digit = credit % 10;
        total += last_digit;
        credit = credit / 10;

        last_digit = credit % 10;
        total += multiply_digit_by_2(last_digit);
        credit = credit / 10;
    }

    int modulo = total % 10;
    if (modulo == 0) 
    {
        return 1;
    } 
    else 
    {
        return 0;
    }
}

int get_length(long credit) // get length of credit, returns int
{
    char str[256];
    sprintf(str, "%ld", credit);
    return strlen(str);
}

void check_long(long credit)
{
    if (!passed_checksum(credit)) 
    {
        printf("INVALID\n");
        return;
    }

    int length = get_length(credit);
    if (length == 15) 
    {
        int first_two_digits = credit / pow(10, 13);
        if (first_two_digits == 34 || first_two_digits == 37) 
        {
            printf("AMEX\n");
            return;
        }
    }

    if (length == 16) 
    {
        int first_two_digits = credit / pow(10, 14);
        if (first_two_digits >= 51 && first_two_digits <= 55) 
        {
            printf("MASTERCARD\n");
            return;
        }
    }

    if (length == 13 || length == 16) // i can check 16 digits again
    {
        int first_digit = credit / pow(10, length - 1);
        if (first_digit == 4) 
        {
            printf("VISA\n");
            return;
        }
    }

    printf("INVALID\n");
}

int main()
{
    long credit = get_long("Number: "); // get credit number
    check_long(credit); // check it. function includes printf
}
