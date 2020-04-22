#include <cs50.h>
#include <stdio.h>

int get_positive_int(void) 
{
    int n;
    do 
    {
        n = get_int("Height: "); // only accepts ints
    } 
    while (n < 1 || n > 8); // get positive int from 1 to 8
    return n;
}

void print_spaces(int n) // prints specified spaces
{
    for (int i = 0; i < n; i++) 
    {
        printf(" ");
    }
}

void print_hashes(int n) // prints specified hashes
{
    for (int i = 0; i < n; i++) 
    {
        printf("#");
    }
}

void print_pyramid(int height)
{
    for (int row = 1; row <= height; row++) 
    {
        print_spaces(height - row);
        print_hashes(row);
        printf("  ");
        print_hashes(row);
        printf("\n");
    }
}

int main(void)
{
    int height = get_positive_int(); // get height from user
    print_pyramid(height); // pass in height to print pyramid
}
