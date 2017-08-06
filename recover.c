#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }
    
    int numOfJPEG = 0; // JPEG file counter
    BYTE *buffer = malloc(512); // buffer to store bytes
    char *infile = argv[1]; 
    char filename[10]; // names for JPEG files
    FILE *img; // Pointer to JPEG file
    bool jpegOpen = false;
    
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    while (fread(buffer, 1, 512, inptr) == 512)  // loop until end of file reached
    {
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0) // Start of a new JPEG
        {
            sprintf(filename, "%03i.jpg", numOfJPEG);
            numOfJPEG++;
            if (jpegOpen)
            {
                fclose(img); // Close file if already open
            }
            img = fopen(filename, "w");
            if (img == NULL)
            {
                fprintf(stderr, "Could not open %s.\n", filename);
                return 2;
            }
            jpegOpen = true;
            fwrite(buffer, 512, 1, img);
        }
        else
        {
            if (jpegOpen)
            {
                // JPEG already found
                fwrite(buffer, 512, 1, img);
            }
        }
    }
    
    // Close all files and free allocated memory
    fclose(inptr);
    if (jpegOpen)
    {
        fclose(img);
    }
    free(buffer);
    
    return 0;
}