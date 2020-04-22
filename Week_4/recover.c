#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure exactly 2 arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // read from infile
    FILE *infile = fopen(argv[1], "r");

    if (infile == NULL)
    {
        printf("Cannot open file");
        return 2;
    }
    else
    {
        printf("File opened\n");
    }

    // image variables
    int image_counter = -1; // will become 0 when first header is found
    char image_name[8]; // create local variable, NOT assign it to some string like image_name = "000.jpg"
    FILE *image_file = NULL; // declare a FILE pointer; to be pointed at image later

    unsigned char buffer[512];



    // keep reading in 512 bytes
    // if fileheader, open new file
    while (fread(buffer, 512, 1, infile) == 1)
    {
        printf("works here!");
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
        {
            if ((buffer[3] & 0xf0) == 0xe0) // beginning of new JPEG file
            {
                if (image_counter >= 0)
                {
                    fclose(image_file);
                }

                image_counter++;
                sprintf(image_name, "%03i.jpg", image_counter);
                printf("before ++count");
                image_file = fopen(image_name, "w");
            }
        }

        if (image_counter >= 0) // it starts at -1 and increments for each image
        {
            fwrite(buffer, 512, 1, image_file);
        }

    }

    fclose(infile);
    fclose(image_file);

    return 0;
}
