/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    float n=atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];
    
    //check if n is within range
    if(n<0.00||n>100.1)
    {
        fprintf(stderr, "float should be greater than 0 and less than or equal to 100\n");
        return 1;
    }

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
    //store height of width of infile
     int h=abs(bi.biHeight);
     int w=bi.biWidth;
     
     //update height and width for outfile
     bi.biHeight=ceil(bi.biHeight*n);
     bi.biWidth=floor(bi.biWidth*n);
     
     //calculate padding for scanlines of outfile
     int pad=(4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
     
     // calculate padding for scanlines of infile
     int padding =  (4 - (w * sizeof(RGBTRIPLE)) % 4) % 4;
     
     //calculate size of headers for outfile
     bi.biSizeImage=((sizeof(RGBTRIPLE)*bi.biWidth) +pad)*abs(bi.biHeight);
     bf.bfSize=bi.biSizeImage+ sizeof(BITMAPFILEHEADER)+ sizeof(BITMAPINFOHEADER);


    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

  //create array to store pixel of scanlines for infile and outfile
   RGBTRIPLE old[w];
   RGBTRIPLE new[bi.biWidth];
   
   int flag=0; 
   int hc=0;

    // iterate over infile's scanlines
    for (int i = 0; i <h; i++)
    {  
        int oc=0;
        // iterate over pixels in scanline
        for (int j = 0; j <w; j++)
        {   
            // temporary storage
            RGBTRIPLE triple;
            // read RGB triple from infile and store in into old
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                old[oc]=triple;
                oc++;
        }
        //write outfile's scanlines
        for(int q=0;q<n;q++)
        {   
            int nc=0;
            if(flag==(int)floor(hc/n))
            
            {    //write each pixel from scanline into outfile
                 for(int r=0;r<bi.biWidth;r++)
                {
                    new[nc]=old[(int)floor(nc/n)];
                    nc++;
                    
                }
                hc++;
                //write padding for outfile
                fwrite(&new,sizeof(new),1,outptr);
                    for (int k=0; k<pad; k++)
                    {
                        fputc(0x00, outptr);
                    }
            }
        }
        flag++;
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }
       

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
