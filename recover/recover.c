#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

typedef uint8_t BYTE;

const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //Open the Memory Card
    FILE *card = fopen(argv[1], "r");
    FILE *jpg = NULL;

    //Allocates memory for block and filename
    BYTE *block = malloc(512 * sizeof(BYTE));
    if(block == NULL)
    {
      return 1;
    }
    char *filename = malloc(8 * sizeof(char));
    if(filename == NULL)
    {
      free(block);
      return 1;
    }
    // Variable to be used for checking if is a start of a jpeg and the count of files already saved
    int isSTART = 0;
    int files_saved = 0;

    while (fread(block, 1, BLOCK_SIZE, card) == 512)
    {
        // Checks if the block is a start of a JPEG file.
        if ((block[0] == 0xff) && (block[1] == 0xd8) && (block[2] = 0xff) && ((block[3] & 0xf0) == 0xe0))
        {
            isSTART = 1;
        }
        else
        {
            isSTART = 0;
        }
        //Checks for garbage data at the start of the rawdata (Added bc ran into problems in an older version)
        if (isSTART == 0 && files_saved == 0)
        {
            continue;
        }

        if (isSTART)
        {
            //Executes always the file isn't the first one.
            if (files_saved != 0)
            {
                fclose(jpg);
            }
            //Adds the formated name to the filename variable
            sprintf(filename, "%03i.jpg", files_saved);
            files_saved++;
            //Opens the new file and write on it.
            jpg = fopen(filename, "w");
            fwrite(block, 1, BLOCK_SIZE, jpg);
        }
        else
        {
            //Just Writes on the file that already exists.
            fwrite(block, 1, BLOCK_SIZE, jpg);
        }
    }
    //Preventing memory leak
    free(block);
    free(filename);
}
