#include <math.h>

#include "helpers.h"


typedef struct
{
    int Gx[3][3];
    int Gy[3][3];
} OPERATOR;

void copyImage(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE dest[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double holder;
    for (int c = 0; c < height; c++) // just kidding
    {
        for (int r = 0; r < width; r++)
        {
            holder = round((image[c][r].rgbtBlue + image[c][r].rgbtGreen + image[c][r].rgbtRed) / 3.0);
            image[c][r].rgbtBlue = holder;
            image[c][r].rgbtGreen = holder;
            image[c][r].rgbtRed = holder;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE placeholder;
    float control;
    for (int c = 0; c < height; c++)
    {
        if (width <= 1)
        {
            return;
        }

        /*  Ternary operator: if width modulo 2 is equal to 0, control is width/2.0, else, it's equal to (width - 1)/2.0
            It's not actually necessary because inside the for-loop I asked for < (less than). If I have width 2, then
            control will evaluate to 1. Everything is fine (because width is 2, so I only want to iterate through the first
            [index = 0] element to swap it with the second), but what if width is 3? So it'll evaluate to 1 too, and everything
            will be fine (because I have three elements, so I want to swap the first with the last and keep the middle where it
            was). This second part is unnecessary because I could just check inside the for loop if in the next iteration my
            r-var is higher than my control var, if so, break. It's probably faster half the times the code runs (?) but
            imo it will exponentially increase the chance of exploding everything in the future.

            tl;dr -> This is a mirroring process (swap the first with the last, the second with the last - 1, etc etc.). I'll keep
            the ternary operator so I don't blow everything in future checks.
        */
        control = (width % 2 == 0) ? width / 2.0 : (width - 1) / 2.0;

        for (int r = 0; r < control; r++)
        {
            placeholder.rgbtBlue = image[c][r].rgbtBlue;
            placeholder.rgbtGreen = image[c][r].rgbtGreen;
            placeholder.rgbtRed = image[c][r].rgbtRed;

            image[c][r].rgbtBlue = image[c][width - 1 - r].rgbtBlue;
            image[c][r].rgbtGreen = image[c][width - 1 - r].rgbtGreen;
            image[c][r].rgbtRed = image[c][width - 1 - r].rgbtRed;

            image[c][width - 1 - r].rgbtBlue = placeholder.rgbtBlue;
            image[c][width - 1 - r].rgbtGreen = placeholder.rgbtGreen;
            image[c][width - 1 - r].rgbtRed = placeholder.rgbtRed;
            // -1 to compensate width-index disparity
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imgCopy[height][width];
    int TOP, BOTTOM, LEFT, RIGHT = 0;
    //Creating a copy of the image so I always have the original pixel values for box-bluring.
    copyImage(height, width, image, imgCopy);

    // Actual bluring process (It could be done using a counter and without lots of repeated code, but I think it's more readable this way...)
    for (int c = 0; c < height; c++)
    {
        for (int r = 0; r < width; r++)
        {
            //COLUMNS PLACEHOLDERS:
            TOP = c - 1;
            BOTTOM = c + 1;

            //ROWS PLACEHOLDERS:
            LEFT =  r - 1;
            RIGHT = r + 1;

            /*The above process isn't core, but allows me to evaluate only once the expressions c +- 1 and r +- 1. It also
            allows the pixel selecting process to be more visible*/

            if (r == 0 && c == 0)
            {
                //Process to blur top left pixel: Takes Itself, 1 right, 1 bottom, 1 diagonal bottom right;
                image[c][r].rgbtRed = round((imgCopy[c][r].rgbtRed + imgCopy[BOTTOM][r].rgbtRed + imgCopy[BOTTOM][RIGHT].rgbtRed +
                                             imgCopy[c][RIGHT].rgbtRed) / 4.0);

                image[c][r].rgbtBlue = round((imgCopy[c][r].rgbtBlue + imgCopy[BOTTOM][r].rgbtBlue + imgCopy[BOTTOM][RIGHT].rgbtBlue +
                                              imgCopy[c][RIGHT].rgbtBlue) / 4.0);

                image[c][r].rgbtGreen = round((imgCopy[c][r].rgbtGreen + imgCopy[BOTTOM][r].rgbtGreen + imgCopy[BOTTOM][RIGHT].rgbtGreen +
                                               imgCopy[c][RIGHT].rgbtGreen) / 4.0);
            }
            else if (r == width - 1 && c == 0)
            {
                //Process to blur top right pixel: Takes Itself, 1 left, 1 bottom, 1 diagonal bottom left;
                image[c][r].rgbtRed = round((imgCopy[c][r].rgbtRed + imgCopy[BOTTOM][r].rgbtRed + imgCopy[BOTTOM][LEFT].rgbtRed +
                                             imgCopy[c][LEFT].rgbtRed) / 4.0);

                image[c][r].rgbtBlue = round((imgCopy[c][r].rgbtBlue + imgCopy[BOTTOM][r].rgbtBlue + imgCopy[BOTTOM][LEFT].rgbtBlue +
                                              imgCopy[c][LEFT].rgbtBlue) / 4.0);

                image[c][r].rgbtGreen = round((imgCopy[c][r].rgbtGreen + imgCopy[BOTTOM][r].rgbtGreen + imgCopy[BOTTOM][LEFT].rgbtGreen +
                                               imgCopy[c][LEFT].rgbtGreen) / 4.0);
            }
            else if (r == 0 && c == height - 1)
            {
                //Process to blur bottom left pixel: Takes itself, 1 right, 1 top, 1 diagonal top right;
                image[c][r].rgbtRed = round((imgCopy[c][r].rgbtRed + imgCopy[TOP][r].rgbtRed + imgCopy[TOP][RIGHT].rgbtRed +
                                             imgCopy[c][RIGHT].rgbtRed) / 4.0);

                image[c][r].rgbtBlue = round((imgCopy[c][r].rgbtBlue + imgCopy[TOP][r].rgbtBlue + imgCopy[TOP][RIGHT].rgbtBlue +
                                              imgCopy[c][RIGHT].rgbtBlue) / 4.0);

                image[c][r].rgbtGreen = round((imgCopy[c][r].rgbtGreen + imgCopy[TOP][r].rgbtGreen + imgCopy[TOP][RIGHT].rgbtGreen +
                                               imgCopy[c][RIGHT].rgbtGreen) / 4.0);
            }
            else if (r == width - 1 && c == height - 1)
            {
                //Process to blur bottom right pixel: Takes itself, 1 left, 1 top, 1 diagonal top left;
                image[c][r].rgbtRed = round((imgCopy[c][r].rgbtRed + imgCopy[TOP][r].rgbtRed + imgCopy[TOP][LEFT].rgbtRed +
                                             imgCopy[c][LEFT].rgbtRed) / 4.0);

                image[c][r].rgbtBlue = round((imgCopy[c][r].rgbtBlue + imgCopy[TOP][r].rgbtBlue + imgCopy[TOP][LEFT].rgbtBlue +
                                              imgCopy[c][LEFT].rgbtBlue) / 4.0);

                image[c][r].rgbtGreen = round((imgCopy[c][r].rgbtGreen + imgCopy[TOP][r].rgbtGreen + imgCopy[TOP][LEFT].rgbtGreen +
                                               imgCopy[c][LEFT].rgbtGreen) / 4.0);
            }
            else if (r == 0 && (c > 0 && c < height - 1))
            {
                //Process to blur left pixels: Takes itself, 1 top, 1 diagonal top right, 1 right, 1 diagonal bottom right and 1 bottom
                image[c][r].rgbtRed = round((imgCopy[c][r].rgbtRed + imgCopy[TOP][r].rgbtRed + imgCopy[TOP][RIGHT].rgbtRed +
                                             imgCopy[c][RIGHT].rgbtRed + imgCopy[BOTTOM][RIGHT].rgbtRed +
                                             imgCopy[BOTTOM][r].rgbtRed) / 6.0);

                image[c][r].rgbtBlue = round((imgCopy[c][r].rgbtBlue + imgCopy[TOP][r].rgbtBlue + imgCopy[TOP][RIGHT].rgbtBlue +
                                              imgCopy[c][RIGHT].rgbtBlue + imgCopy[BOTTOM][RIGHT].rgbtBlue + imgCopy[BOTTOM][r].rgbtBlue) / 6.0);

                image[c][r].rgbtGreen = round((imgCopy[c][r].rgbtGreen + imgCopy[TOP][r].rgbtGreen + imgCopy[TOP][RIGHT].rgbtGreen +
                                               imgCopy[c][RIGHT].rgbtGreen + imgCopy[BOTTOM][RIGHT].rgbtGreen + imgCopy[BOTTOM][r].rgbtGreen) / 6.0);
            }
            else if (r == width - 1 && (c > 0 && c < height - 1))
            {
                //Process to blur right pixels: Takes itself, 1 top, 1 diagonal top left, 1 left, 1 diagonal bottom left and 1 bottom
                image[c][r].rgbtRed = round((imgCopy[c][r].rgbtRed + imgCopy[TOP][r].rgbtRed + imgCopy[TOP][LEFT].rgbtRed + imgCopy[c][LEFT].rgbtRed
                                             + imgCopy[BOTTOM][LEFT].rgbtRed + imgCopy[BOTTOM][r].rgbtRed) / 6.0);

                image[c][r].rgbtBlue = round((imgCopy[c][r].rgbtBlue + imgCopy[TOP][r].rgbtBlue + imgCopy[TOP][LEFT].rgbtBlue +
                                              imgCopy[c][LEFT].rgbtBlue + imgCopy[BOTTOM][LEFT].rgbtBlue + imgCopy[BOTTOM][r].rgbtBlue) / 6.0);

                image[c][r].rgbtGreen = round((imgCopy[c][r].rgbtGreen + imgCopy[TOP][r].rgbtGreen + imgCopy[TOP][LEFT].rgbtGreen +
                                               imgCopy[c][LEFT].rgbtGreen + imgCopy[BOTTOM][LEFT].rgbtGreen + imgCopy[BOTTOM][r].rgbtGreen) / 6.0);
            }
            else if ((r > 0 && r < width - 1) && c == 0)
            {
                //Process to blur top pixels: Takes itself, 1 left, 1 diagonal left bottom, 1 bottom, 1 diagonal right bottom and 1 right;
                image[c][r].rgbtRed = round((imgCopy[c][r].rgbtRed + imgCopy[c][LEFT].rgbtRed + imgCopy[BOTTOM][LEFT].rgbtRed +
                                             imgCopy[BOTTOM][r].rgbtRed + imgCopy[BOTTOM][RIGHT].rgbtRed + imgCopy[c][RIGHT].rgbtRed) / 6.0);

                image[c][r].rgbtBlue = round((imgCopy[c][r].rgbtBlue + imgCopy[c][LEFT].rgbtBlue + imgCopy[BOTTOM][LEFT].rgbtBlue +
                                              imgCopy[BOTTOM][r].rgbtBlue + imgCopy[BOTTOM][RIGHT].rgbtBlue + imgCopy[c][RIGHT].rgbtBlue) / 6.0);

                image[c][r].rgbtGreen = round((imgCopy[c][r].rgbtGreen + imgCopy[c][LEFT].rgbtGreen + imgCopy[BOTTOM][LEFT].rgbtGreen +
                                               imgCopy[BOTTOM][r].rgbtGreen + imgCopy[BOTTOM][RIGHT].rgbtGreen + imgCopy[c][RIGHT].rgbtGreen) / 6.0);
            }
            else if ((r > 0 && r < width - 1) && c == height - 1)
            {
                //Process to blur bottom pixels: Takes itself, 1 left, 1 diagonal left top, 1 top, 1 diagonal right top and 1 right
                image[c][r].rgbtRed = round((imgCopy[c][r].rgbtRed + imgCopy[c][LEFT].rgbtRed + imgCopy[TOP][LEFT].rgbtRed + imgCopy[TOP][r].rgbtRed
                                             + imgCopy[TOP][RIGHT].rgbtRed + imgCopy[c][RIGHT].rgbtRed) / 6.0);

                image[c][r].rgbtBlue = round((imgCopy[c][r].rgbtBlue + imgCopy[c][LEFT].rgbtBlue + imgCopy[TOP][LEFT].rgbtBlue +
                                              imgCopy[TOP][r].rgbtBlue + imgCopy[TOP][RIGHT].rgbtBlue + imgCopy[c][RIGHT].rgbtBlue) / 6.0);

                image[c][r].rgbtGreen = round((imgCopy[c][r].rgbtGreen + imgCopy[c][LEFT].rgbtGreen + imgCopy[TOP][LEFT].rgbtGreen +
                                               imgCopy[TOP][r].rgbtGreen + imgCopy[TOP][RIGHT].rgbtGreen + imgCopy[c][RIGHT].rgbtGreen) / 6.0);
            }
            else
            {
                //Process to blur middle pixels: Takes itself, 1 left, 1 top left, 1 top, 1 top right, 1 right, 1 bottom right, 1 bottom, 1 bottom left.
                image[c][r].rgbtRed = round((imgCopy[c][r].rgbtRed + imgCopy[c][LEFT].rgbtRed + imgCopy[TOP][LEFT].rgbtRed + imgCopy[TOP][r].rgbtRed
                                             + imgCopy[TOP][RIGHT].rgbtRed + imgCopy[c][RIGHT].rgbtRed + imgCopy[BOTTOM][RIGHT].rgbtRed +
                                             imgCopy[BOTTOM][r].rgbtRed + imgCopy[BOTTOM][LEFT].rgbtRed) / 9.0);

                image[c][r].rgbtBlue = round((imgCopy[c][r].rgbtBlue + imgCopy[c][LEFT].rgbtBlue + imgCopy[TOP][LEFT].rgbtBlue +
                                              imgCopy[TOP][r].rgbtBlue + imgCopy[TOP][RIGHT].rgbtBlue + imgCopy[c][RIGHT].rgbtBlue + imgCopy[BOTTOM][RIGHT].rgbtBlue +
                                              imgCopy[BOTTOM][r].rgbtBlue + imgCopy[BOTTOM][LEFT].rgbtBlue) / 9.0);

                image[c][r].rgbtGreen = round((imgCopy[c][r].rgbtGreen + imgCopy[c][LEFT].rgbtGreen + imgCopy[TOP][LEFT].rgbtGreen +
                                               imgCopy[TOP][r].rgbtGreen + imgCopy[TOP][RIGHT].rgbtGreen + imgCopy[c][RIGHT].rgbtGreen + imgCopy[BOTTOM][RIGHT].rgbtGreen +
                                               imgCopy[BOTTOM][r].rgbtGreen + imgCopy[BOTTOM][LEFT].rgbtGreen) / 9.0);
            }
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    OPERATOR kernels = {.Gx = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}, .Gy = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}}};
    RGBTRIPLE imgCopy[height][width];

    int gysRed = 0, gysBlue = 0, gysGreen = 0, gxsRed = 0, gxsBlue = 0, gxsGreen = 0;

    copyImage(height, width, image, imgCopy);

    /* Here I'll iterate over each pixel in the image and over the 9 pixels that form the 3x3 box to show what I was talking
    about in the blur function and to save time. */
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            /* Here if we have pixel (h,w) = (0,1) (it means, the second pixel of the first row) I want to iterate around this pixel
            and check which ones I have to compute (I mean, I have to check for pixel. (0,0), (-1,0), (-1,1), (-1,2), (0,2), (1,2),
            (1,1), (1,0) and discover, count and compute valid ones.

            There are two possible cases that should be excluded while iterating (which is the same as considering them black):
                I) Pixel requested is lower than 0;
                II) Pixel requested is higher than height - 1 or width - 1; (Because of width-index disparity)

            We could consider the next iteration as a submatrix (or subarray) that happens to exists around the selected pixel
            (below sh stands for sub height and sw for sub width). Then it has relative coordinates to the selected pixel
            (top left is (h-1,w-1), top is (h-1,w), top right is (h-1, w+1), right is (h, w+1), bottom right is (h+1,w+1),
            bottom is (h+1,w), bottom left is (h+1,w-1), left is (h, w - 1)) and these relative coordinates can't exceed the I
            and II boundaries. */

            gysRed = 0, gysBlue = 0, gysGreen = 0, gxsRed = 0, gxsBlue = 0, gxsGreen = 0;

            for (int sh = -1; sh <= 1; sh++)
            {
                for (int sw = -1; sw <= 1; sw++)
                {
                    if (sh + h < 0 || sh + h > height - 1 || sw + w < 0 || sw + w > width - 1)
                    {
                        continue; //It'll make the iterator go to the next iteration. If I wanted to make it faster I could
                    }

                    // Gy Scalar Values
                    gysRed += imgCopy[h + sh][w + sw].rgbtRed * kernels.Gy[sh + 1][sw + 1];
                    gysGreen += imgCopy[h + sh][w + sw].rgbtGreen * kernels.Gy[sh + 1][sw + 1];
                    gysBlue += imgCopy[h + sh][w + sw].rgbtBlue * kernels.Gy[sh + 1][sw + 1];

                    // Gx Scalar Values

                    gxsRed += imgCopy[h + sh][w + sw].rgbtRed * kernels.Gx[sh + 1][sw + 1];
                    gxsGreen += imgCopy[h + sh][w + sw].rgbtGreen * kernels.Gx[sh + 1][sw + 1];
                    gxsBlue += imgCopy[h + sh][w + sw].rgbtBlue * kernels.Gx[sh + 1][sw + 1];

                    //Write Pixel in the Img
                    int r = round(sqrt(pow(gysRed, 2) + pow(gxsRed, 2))), g = round(sqrt(pow(gysGreen, 2) + pow(gxsGreen, 2))),
                        b = round(sqrt(pow(gysBlue, 2) + pow(gxsBlue, 2)));
                    image[h][w].rgbtRed = r <= 255 ? r : 255;
                    image[h][w].rgbtGreen = g <= 255 ? g : 255;
                    image[h][w].rgbtBlue = b <= 255 ? b : 255;
                }
            }
        }
    }
}

void copyImage(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE dest[height][width])
{
    for (int col = 0; col < height; col++)
    {
        for (int row = 0; row < width; row++)
        {
            dest[col][row].rgbtBlue = image[col][row].rgbtBlue;
            dest[col][row].rgbtGreen = image[col][row].rgbtGreen;
            dest[col][row].rgbtRed = image[col][row].rgbtRed;
        }
    }
}
