/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize factor infile outfile\n");
        return 1;
    }
    
    // remember and check factor
    double f = atof(argv[1]);
    if (f <= 0.0 || f > 100)
    {
        fprintf(stderr, "Invalid number\n");
        return 1;
    }
    
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // Update header information
    // Calculate height and width
    BITMAPFILEHEADER bfNew = bf;
    BITMAPINFOHEADER biNew = bi;
    
    // Calculate and normalize numbers of reads and writes
    int numOfReads = 1;
    int numOfWrites = 1;
    if (f < 1.0) 
    {
        numOfReads = floor(1.0 / f + 0.5);
        numOfWrites = 1;
        biNew.biWidth = floor(biNew.biWidth / numOfReads + 0.5);
        biNew.biHeight = floor(biNew.biHeight / numOfReads + 0.5);
    }
    else
    {
        numOfReads = 1;
        numOfWrites = floor(f + 0.5);
        biNew.biWidth = biNew.biWidth * numOfWrites;
        biNew.biHeight = biNew.biHeight * numOfWrites;
    }

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int newPadding = (4 - (biNew.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // Calculate the size of the image
    biNew.biSizeImage = ((sizeof(RGBTRIPLE) * biNew.biWidth) + padding) * abs(biNew.biHeight);
    //Calculate the size of the file
    bfNew.bfSize = biNew.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfNew, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biNew, sizeof(BITMAPINFOHEADER), 1, outptr);
    


    // iterate over infile's scanlines
    int biHeight = abs(bi.biHeight);
    int i = 0;
    int numOfRowsWritten = numOfWrites;
    while (i < biHeight)
    {

        // temporary storage
        RGBTRIPLE triple;
        RGBTRIPLE tripleToWrite;
        
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            // Write RGB triple to outfile
            if (i % numOfReads == 0 && j % numOfReads == 0) // Decide to write or to skip
            {
                tripleToWrite = triple;
                // write RGB triple to outfile
                for (int k = 0; k < numOfWrites; k++)
                {
                    fwrite(&tripleToWrite, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

        }
        
        // Add padding to outfile if the row was written
        if (i % numOfReads == 0) {
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }
        }
        
        if (numOfRowsWritten > 1)
        {
            // Move infile pointer a row back to read and write the row again
            long int offset = sizeof(RGBTRIPLE) * bi.biWidth;
            fseek(inptr, -offset, SEEK_CUR);
            numOfRowsWritten--;
        }
        else
        {
            // Continue reading infile
            fseek(inptr, padding, SEEK_CUR); // skip over padding, if any
            numOfRowsWritten = numOfWrites; // Set rows counter for a new row
            i++;
        }

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
