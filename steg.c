/* Name : Suman Kafle
CS 241
lab 9 steg.c*/

#include <stdio.h>
#include <stdlib.h>

/****************************************************************************/
/* take the int and the byte
   this function change the last 2 bits of the bytes from the bits of int n                                                    */
/****************************************************************************/
void modifier(int n, unsigned char bytes[])
{
  bytes[0] = (bytes[0] & 0xFC) | (n & 0x03);
  bytes[1] = (bytes[1] & 0xFC) | ((n >> 2) & 0x03);
  bytes[2] = (bytes[2] & 0xFC) | ((n >> 4) & 0x03);
  bytes[3] = (bytes[3] & 0xFC) | ((n >> 6) & 0x03);
}

/**********************************************************************/
/* Take 4 bytes from an unsigned char array and assemble them into an */
/* int where the first element of the array is the least significant  */
/* byte of the int.                                                   */
/**********************************************************************/
int getIntFromArray(unsigned char bytes[])
{
    int n =
          bytes[0] |
          bytes[1] << 8 |
          bytes[2] << 16 |
      bytes[3] << 24;
    return n;
}

/* Copy a bitmap file without red */
int main(int argc, char** argv)
{
  char* infilename = argv[1];
  char* outfilename = argv[2];

  unsigned char header[54];

  FILE* in = fopen(infilename, "rb");
  FILE* out = fopen(outfilename, "wb");

  int pixelDataSize;
  int values;
  int EOFFlag =1;
  int i;

  /* read header into array */
  fread(header, 1, 54, in);

  /* is this really a bitmap? */
  if(header[0] != 'B' || header[1] != 'M')
    {
      printf("Input file is not a bitmap\n");
    }

  /* Is the header size what we expect? */
  if(getIntFromArray(&header[10]) != 54)
    {
      printf("Unexpected header size\n");
    }

  /* How many bits per pixel? Expecting 24*/
  if(!(header[28] == 24 && header[29] == 0))
    {
      printf("Unexpected number of bits/pixel\n");
    }

  pixelDataSize = getIntFromArray(&header[34]);

  /* write header to output file */
  fwrite(header, 1, sizeof(header), out);

  /* Read RGB data from original, copy without red */
  for(i = 0; i < pixelDataSize/4; ++i)
    {
      unsigned char bytes[4];

      /* color order is BGR */
      fread(bytes, 1, 4, in);

      /* if the input values is not  end of the file the modifier function is called*/
      if ( (values = getchar()) !=EOF )
	{
	  modifier(values,bytes);
	}

      /* if it end  modifier takes 0 as an input rather than any of the */
      else if(EOFFlag)
	{
	  EOFFlag =0;
	  modifier(0,bytes);
	}
      fwrite(bytes, 1, 4, out);
    }

  fclose(in);
  fclose(out);
  return 0;
}
