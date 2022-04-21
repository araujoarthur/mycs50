#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height = 0;
    do
    {
        height = get_int("How tall do you want it to be? ");
    }
    while (height > 8 || height < 1);
    // Each how
    for (int i = 0; i < height; i++)
    {
        // Each column
        for (int j = 1; j <= height; j += 1)
        {
            // Print as many # as i.
            if (j < height - i)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        // Fill the gap
        printf("  ");
        for (int k = 0; k <= i; k += 1)
        {
            printf("#");
        }
        printf("\n");
    }
}
