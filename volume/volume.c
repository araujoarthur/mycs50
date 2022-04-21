// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t h[HEADER_SIZE]; // Array with 44 bytes (8*44 bits)
    fread(&h, HEADER_SIZE, 1, input);
    fwrite(&h, HEADER_SIZE, 1, output);

    // TODO: Read samples from input file and write updated data to output file
    fseek(input, 44, SEEK_SET); // Just making sure I'm at byte 45
    int16_t amp = -1;

    while (fread(&amp, 2, 1, input))
    {
        amp = amp * factor;
        fwrite(&amp, 2, 1, output);
    }
    // Close files
    fclose(input);
    fclose(output);
}
