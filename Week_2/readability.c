#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void process_text(string text) // printf after getting text input from user
{
    int words = 1, letters = 0, sentences = 0; // starts with words = 1 as last word has no space
    for (int i = 0; i < strlen(text); i++)
    {
        if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122))
        {
            letters++;
        }
        if (text[i] == ' ') // space denotes word.
        {
            words++;
        }
        if (text[i] == '!' || text[i] == '?' || text[i] == '.')
        {
            sentences++;
        }
    }

    float L = 100.0 * letters / words; // 100.0 converts whole thing to float
    float S = 100.0 * sentences / words; // avg no. of sentences per 100 words

    float index = 0.0588 * L - 0.296 * S - 15.8;
    int int_index = (int) round(index);

    if (int_index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (int_index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", int_index);
    }
}

int main(void)
{
    string text = get_string("Text: "); // get string
    process_text(text);
}