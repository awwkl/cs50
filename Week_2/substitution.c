#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *upper_string(char *key) // convert key to all uppercase, for later convenience
{
    int len = strlen(key);
    for (int i = 0; i < len; i++)
    {
        key[i] = toupper(key[i]);
    }
    return key;
}

int is_valid_key(char *key) // key must be length 26, all alphabetic, no repeated
{


    if (strlen(key) != 26) // ensure key length is 26
    {
        return 0;
    }

    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("All characters must be alphabets\n");
            return 0;
        }
    }

    char copy[27];
    strcpy(copy, key); // make a copy of the key, because qsort sorts IN PLACE

    int codes[26];
    for (int i = 0; i < 26; i++)
    {
        codes[i] = 0;
    }

    for (int i = 0; i < 26; i++)
    {
        int n = (int) key[i] - 65;
        if (codes[n] == 0)
        {
            codes[n] = 1;
        }
        else if (codes[n] == 1)
        {
            printf("No duplicate characters!\n");
            return 0;
        }
    }




    return 1;
}



void print_ciphertext(char *p, int *diff) // print ciphertext, given plaintext p and diff array
{
    printf("ciphertext: ");

    int len = strlen(p);
    for (int i = 0; i < len; i++)
    {
        if ((p[i] >= 65 && p[i] <= 90) || (p[i] >= 97 && p[i] <= 122))
        {
            int char_code = (int) p[i];
            int diff_code;
            if (char_code <= 90)
            {
                diff_code = char_code - 65;
            }
            else
            {
                diff_code = char_code - 97;
            }
            printf("%c", (int) p[i] + diff[diff_code]);
        }
        else
        {
            printf("%c", p[i]);
        }
    }

}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    char *key = argv[1]; // set key as command line argument
    key = upper_string(key); // convert key to uppercase

    if (!is_valid_key(key)) // length 26, all alphabetic, no repeated
    {
        printf("Key is not valid.\n");
        return 1;
    }

    int diff[26]; // diff array; key=BCDEF => diff={1,1,1,1,1}
    for (int i = 0; i < 26; i++)
    {
        diff[i] = (int)key[i] - (65 + i); // B=66, A=65; so diff of 1
    }

    char plaintext[999];
    printf("plaintext: ");
    fgets(plaintext, 999, stdin);
    print_ciphertext(plaintext, diff);
}