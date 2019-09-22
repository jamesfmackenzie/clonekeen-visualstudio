#ifndef TGA_H
#define TGA_H

#define TGA_NONE		0
#define TGA_INDEXED		1
#define TGA_RGB			2
#define TGA_GREYSCALE	3

#include  <Windows.h>

typedef struct
{
    UCHAR identsize;           // size of ID field that follows 18 UCHAR header (usually 0)
    UCHAR colourmaptype;       // type of colour map 0=none, 1=has palette
    UCHAR imagetype;           // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    UINT colourmapstart;       // first colour map entry in palette
    UINT colourmaplength;      // number of colours in palette
    UCHAR colourmapbits;       // number of bits per palette entry 15,16,24,32

    UINT xstart;               // image x origin
    UINT ystart;               // image y origin
    UINT width;                // image width in pixels
    UINT height;               // image height in pixels
    UCHAR bpp;	               // image bits per pixel 8,16,24,32
    UCHAR descriptor;          // image descriptor bits (vh flip bits)
    
    // pixel data follows header    
} TGA_HEADER;

#endif
