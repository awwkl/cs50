#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++) // iterate thru all pixels
        {
            // rgbtGray is average of RGB values
            int rgbtGray = round((image[r][c].rgbtBlue + image[r][c].rgbtGreen + image[r][c].rgbtRed) / 3.0);
            image[r][c].rgbtBlue = rgbtGray;
            image[r][c].rgbtGreen = rgbtGray;
            image[r][c].rgbtRed = rgbtGray;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int r = 0; r < height; r++)
    {
        RGBTRIPLE reverse_row[width]; // create reversed ROW (array) of  pixels
        for (int c = 0; c < width; c++)
        {
            reverse_row[c].rgbtBlue = image[r][width - 1 - c].rgbtBlue;
            reverse_row[c].rgbtGreen = image[r][width - 1 - c].rgbtGreen;
            reverse_row[c].rgbtRed = image[r][width - 1 - c].rgbtRed;
        }

        for (int c = 0; c < width; c++)
        {
            image[r][c].rgbtBlue = reverse_row[c].rgbtBlue;
            image[r][c].rgbtGreen = reverse_row[c].rgbtGreen;
            image[r][c].rgbtRed = reverse_row[c].rgbtRed;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurred[height][width];

    // ignore the pixels on the 4 EDGES first
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            int sumBlue = 0, sumGreen = 0, sumRed = 0;
            float counter = 0.0;

            for (int a = -1; a <= 1; a++)
            {
                if (r + a < 0 || r + a > height - 1) // if outside top or bottom edge
                {
                    continue;
                }

                for (int b = -1; b <= 1; b++)
                {
                    if (b + c < 0 || b + c > width - 1) // if outside left or right edge
                    {
                        continue;
                    }

                    sumBlue += image[r + a][b + c].rgbtBlue;
                    sumGreen += image[r + a][b + c].rgbtGreen;
                    sumRed += image[r + a][b + c].rgbtRed;
                    counter++;
                }
            }

            // counter is float so it will not default to rounding down e.g. 5/3 = 1
            blurred[r][c].rgbtBlue = round(sumBlue / counter);
            blurred[r][c].rgbtGreen = round(sumGreen / counter);
            blurred[r][c].rgbtRed = round(sumRed / counter);
        }
    }

    for (int r = 0; r < height; r++) // copy all pixels back to image
    {
        for (int c = 0; c < width; c++)
        {
            image[r][c].rgbtBlue = blurred[r][c].rgbtBlue;
            image[r][c].rgbtGreen = blurred[r][c].rgbtGreen;
            image[r][c].rgbtRed = blurred[r][c].rgbtRed;
        }
    }
}

//helper for function 'edges'
// calculate Sobel value, given GxGreen, GyGreen ... GxBlue etc.
int calc_sobel(int x, int y)
{
    int val = round(sqrt((float) x * x + y * y));
    if (val > 255)
    {
        return 255; // cap at 255
    }
    return val;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width]; // create temp bitmap

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            // prepare Gx and Gy values for RGB
            int GxBlue = 0, GxGreen = 0, GxRed = 0;
            int GyBlue = 0, GyGreen = 0, GyRed = 0;

            // calculate Gx values
            if (c > 0) // i.e. column greater than 0
            {
                GxBlue += (-2) * image[r][c - 1].rgbtBlue;
                GxGreen += (-2) * image[r][c - 1].rgbtGreen;
                GxRed += (-2) * image[r][c - 1].rgbtRed;

                if (r > 0) // i.e. row greater than 0
                {
                    GxBlue += (-1) * image[r - 1][c - 1].rgbtBlue;
                    GxGreen += (-1) * image[r - 1][c - 1].rgbtGreen;
                    GxRed += (-1) * image[r - 1][c - 1].rgbtRed;
                }
                if (r < height - 1)
                {
                    GxBlue += (-1) * image[r + 1][c - 1].rgbtBlue;
                    GxGreen += (-1) * image[r + 1][c - 1].rgbtGreen;
                    GxRed += (-1) * image[r + 1][c - 1].rgbtRed;
                }
            }
            if (c < width - 1)
            {
                GxBlue += (2) * image[r][c + 1].rgbtBlue;
                GxGreen += (2) * image[r][c + 1].rgbtGreen;
                GxRed += (2) * image[r][c + 1].rgbtRed;

                if (r > 0)
                {
                    GxBlue += (1) * image[r - 1][c + 1].rgbtBlue;
                    GxGreen += (1) * image[r - 1][c + 1].rgbtGreen;
                    GxRed += (1) * image[r - 1][c + 1].rgbtRed;
                }
                if (r < height - 1)
                {
                    GxBlue += (1) * image[r + 1][c + 1].rgbtBlue;
                    GxGreen += (1) * image[r + 1][c + 1].rgbtGreen;
                    GxRed += (1) * image[r + 1][c + 1].rgbtRed;
                }
            }

            // calculate Gy values
            if (r > 0)
            {
                GyBlue += (-2) * image[r - 1][c].rgbtBlue;
                GyGreen += (-2) * image[r - 1][c].rgbtGreen;
                GyRed += (-2) * image[r - 1][c].rgbtRed;

                if (c > 0)
                {
                    GyBlue += (-1) * image[r - 1][c - 1].rgbtBlue;
                    GyGreen += (-1) * image[r - 1][c - 1].rgbtGreen;
                    GyRed += (-1) * image[r - 1][c - 1].rgbtRed;
                }

                if (c < width - 1)
                {
                    GyBlue += (-1) * image[r - 1][c + 1].rgbtBlue;
                    GyGreen += (-1) * image[r - 1][c + 1].rgbtGreen;
                    GyRed += (-1) * image[r - 1][c + 1].rgbtRed;
                }
            }

            if (r < height - 1)
            {
                GyBlue += (2) * image[r + 1][c].rgbtBlue;
                GyGreen += (2) * image[r + 1][c].rgbtGreen;
                GyRed += (2) * image[r + 1][c].rgbtRed;

                if (c > 0)
                {
                    GyBlue += (1) * image[r + 1][c - 1].rgbtBlue;
                    GyGreen += (1) * image[r + 1][c - 1].rgbtGreen;
                    GyRed += (1) * image[r + 1][c - 1].rgbtRed;
                }

                if (c < width - 1)
                {
                    GyBlue += (1) * image[r + 1][c + 1].rgbtBlue;
                    GyGreen += (1) * image[r + 1][c + 1].rgbtGreen;
                    GyRed += (1) * image[r + 1][c + 1].rgbtRed;
                }
            }


            temp[r][c].rgbtBlue = calc_sobel(GxBlue, GyBlue);
            temp[r][c].rgbtGreen = calc_sobel(GxGreen, GyGreen);
            temp[r][c].rgbtRed = calc_sobel(GxRed, GyRed);
        }
    }

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            image[r][c].rgbtBlue = temp[r][c].rgbtBlue;
            image[r][c].rgbtGreen = temp[r][c].rgbtGreen;
            image[r][c].rgbtRed = temp[r][c].rgbtRed;
        }
    }
}
